#include "Config.hxx"
#include "Logging.hxx"
#include "HttpRequest.hxx"
#include "Md5Validator.hxx"
#include "String.hxx"

#include <string.h>
#include <errno.h>
#include <yaml.h>
#include <stdint.h>
#include <stdio.h>

namespace Punisher {

Config::Config() {
	threads = 1;
	quiet = false;
	errors = false;
	validation = false;
}
Config::~Config() {
}

int
Config::GetThreads() const {
	return threads;
}

void
Config::SetThreads(int threads) {
	LOGGING_DEBUG("Requested [%d] threads", threads);
	this->threads = (threads < 1) ? sysconf(_SC_NPROCESSORS_ONLN) : threads;
	LOGGING_DEBUG("Got [%d] threads", this->threads);
}

int
Config::GetDelay() const {
	return delay;
}

void
Config::SetDelay(int delay) {
	this->delay = delay;
}

int
Config::GetConcurrent() const {
	return concurrent;
}
void
Config::SetConcurrent(int concurrent) {
	this->concurrent = concurrent;
}

bool
Config::GetQuiet() const {
	return quiet;
}
void
Config::SetQuiet(bool quiet) {
	this->quiet = quiet;
}

bool
Config::GetValidation() const {
	return validation;
}
void
Config::SetValidation(bool validation) {
	this->validation = validation;
}

bool
Config::GetErrors() const {
	return errors;
}

void
Config::SetErrors(bool errors) {
	this->errors = errors;
}

size_t
Config::GetIterations() const {
	return iterations; 
}
void
Config::SetIterations(size_t iterations) {
	this->iterations = iterations;
}

const std::vector<Request*>&
Config::GetRequests() const {
	return requests;
}
const Request*
Config::GetRequest(unsigned int id, uint64_t& next_request) const {
	//this is my static work-allocation function for threads.
	//the primary goal is to be fast while still spreading the requests
	//out somewhat evenly across threads.
	uint64_t idx = id + next_request;

	//next, check and make sure our computed index exists
	if (idx > requests.size() - 1) {
		//if we have more threads than requests, switch to a modulo-style
		//index computation
		(id > requests.size() - 1) ? idx = (id % requests.size()) : idx = id;
		next_request = 0;
	} else {
		//set the next request to skip ahead by the number of threads
		idx = id + next_request;
		next_request += threads;
	}

	return requests[idx];
}

time_t
Config::GetTimelimit() const {
	return timelimit;
}
void
Config::SetTimelimit(time_t timelimit) {
	this->timelimit = timelimit;
}

bool
Config::GetKeepalive() const {
	return keepalive;
}
void
Config::SetKeepalive(bool keepalive) {
	this->keepalive = keepalive;
}

bool
Config::GetFastmode() const {
	return fastmode;
}
void
Config::SetFastmode(bool fastmode) {
	this->fastmode = fastmode;
}

M::String
Config::GetScript() const {
	return script;
}
void
Config::SetScript(const M::String& script) {
	return SetScript(script.ToCString());
}
void
Config::SetScript(const char* script) {
	this->script = script;
	ParseScript();
}

bool
Config::ParseScript() {
	Logging::Info("Loading script, please wait...");

	FILE* infile = fopen(script.ToCString(), "r");
	if (infile == NULL) {
		Logging::Error("Error reading script: %s", strerror(errno));
		return false;
	}

	//init libyaml
	yaml_parser_t parser;
	yaml_document_t document;
	memset(&parser, 0, sizeof(parser));
	memset(&document, 0, sizeof(document));
	int done = 0;

	yaml_parser_initialize(&parser);
	yaml_parser_set_input_file(&parser, infile);

	//main parsing loop
	while (!done) {
		//load document
		if (!yaml_parser_load(&parser, &document)) {
			Logging::Error("yaml_parser_parse(): %s", parser.problem);
			yaml_parser_delete(&parser);
			return false;
		}

		if (!yaml_document_get_root_node(&document)) {
			done = 1;
		} else {
			ParseDocument(&document);
			yaml_document_delete(&document);
		}
	}

	yaml_parser_delete(&parser);

	return true;
}


bool
Config::AddRequest(const char* url) {
	//TODO: This needs to be made cross-protocol somehow...
	Request* request = NULL;
	request = new HttpRequest();

	bool ret = false;
	if (true == (ret = request->SetField(HTTP_FIELD_URL, url))) {
		//request->Process();
		requests.push_back(request);
	}

	return ret;
}

void
Config::ParseDocument(const yaml_document_t* document) {
	LOGGING_DEBUG("Config::ParseDocument(): Start");
	yaml_node_t* node;
	Request* request = NULL;

	//loop through each node in the document
	for (node = document->nodes.start; node < document->nodes.top; node++) {
		//we only want scalar nodes
		if (node->type != YAML_SCALAR_NODE) {
			continue;
		}

		LOGGING_DEBUG("Config::ParseDocument(): Node: %s",
		               (const char*)node->data.scalar.value);

		//create a new request object for 'request' node
		if (0 == strncmp((const char*)node->data.scalar.value, "http", 4)) {
			//if we were working with a previous request before we got this new
			//request, then we need to save off the old one, then continue with
			//making the new one.
			//pre-generate request and save to queue
			if (request != NULL) {
				LOGGING_DEBUG("Config::ParseDocument(): Adding request to queue");
				//request->Process();
				requests.push_back(request);
				request = NULL;
			}

			request = new HttpRequest();
			continue;
		}

		//if we've gotten this far, we need to have a current request
		if (request == NULL) {
			Logging::Error("Config::ParseDocument(): We have no request. Skipping");
			continue;
		}

		//assign url to request
		if (0 == strncmp((const char*)node->data.scalar.value, "url", 3)) {
			LOGGING_DEBUG("Config::ParseDocument(): Got url token");
			//the next node will be the url value
			if ((node + 1) < document->nodes.top) {
				++node;
			}
			LOGGING_DEBUG("Config::ParseDocument(): Request::SetUri(%s)",
			               (const char*)node->data.scalar.value);
			if (!request->SetField(HTTP_FIELD_URL,
			                       (const char*)node->data.scalar.value)) {
				LOGGING_DEBUG("Config::ParseDocuemtn(): Invalid URI: %s",
				               (const char*)node->data.scalar.value);
				delete(request);
				request = NULL;
				continue;
			}
		}

		//assign md5 validation to request
		if (0 == strncmp((const char*)node->data.scalar.value, "md5", 3)) {
			LOGGING_DEBUG("Config::ParseDocument(): Got md5 token");
			//the next node should be the md5 ascii string
			if ((node + 1) < document->nodes.top) {
				++node;
			}
			LOGGING_DEBUG("Config::ParseDocument(): Md5Validator(%s)",
			               (const char*)node->data.scalar.value);

			if (true == GetValidation()) {
				const char* hash = (const char*)node->data.scalar.value;
				request->GetValidator() = new Md5Validator(hash);
			}
		}

		//assign method to request
		if (0 == strncmp((const char*)node->data.scalar.value, "method", 6)) {
			LOGGING_DEBUG("Config::ParseDocument(): Got method token");
			//the next node should be the method in string form
			if ((node + 1) < document->nodes.top) {
				++node;
			}
			LOGGING_DEBUG("Config::ParseDocument(): Request::Method::Set(%s)",
			               (const char*)node->data.scalar.value);
			request->SetField(HTTP_FIELD_METHOD,
			                  ((const char*)node->data.scalar.value));
		}

		//assign version to request
		if (0 == strncmp((const char*)node->data.scalar.value, "version", 7)) {
			LOGGING_DEBUG("Config::ParseDocument(): Got version token");
			//the next node should be the version in string form
			if ((node + 1) < document->nodes.top) {
				++node;
			}
			LOGGING_DEBUG("Config::ParseDocuemtn(): Request::Version::Set(%s)",
			               (const char*)node->data.scalar.value);
			request->SetField(HTTP_FIELD_VERSION,
			                  ((const char*)node->data.scalar.value));
		}

		LOGGING_DEBUG("Config::ParseDocument(): Processing next node...");
	}
	//pre-generate request and save to queue
	//LOGGING_DEBUG("Config::ParseDocument(): Adding request to queue");
	//request->Process();
	requests.push_back(request);
	request = NULL;
}

}
