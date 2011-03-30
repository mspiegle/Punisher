/*
 * Punisher
 * by Michael Spiegle
 * 09.03.09
 *
 * Config.hxx
 */

#ifndef _PUNISHER_CONFIG_HXX_
#define _PUNISHER_CONFIG_HXX_

#include "Request.hxx"
#include "String.hxx"

#include <vector>
#include <time.h>
#include <yaml.h>
#include <stdint.h>

namespace Punisher {

class Config {
	private:
		int                   delay;
		int                   threads;
		int                   concurrent;
		bool                  validation;
		bool                  fastmode;
		bool                  keepalive;
		bool                  quiet;
		bool                  errors;
		time_t                timelimit;
		size_t                iterations;
		M::String             script;
		std::vector<Request*> requests;


	public:
		Config();
		~Config();

		int  GetThreads() const;
		void SetThreads(int threads);

		int  GetDelay() const;
		void SetDelay(int delay);

		bool GetQuiet() const;
		void SetQuiet(bool quiet);

		bool GetErrors() const;
		void SetErrors(bool errors);

		bool GetValidation() const;
		void SetValidation(bool validation);

		int  GetConcurrent() const;
		void SetConcurrent(int concurrent);

		size_t  GetIterations() const;
		void SetIterations(size_t iterations);

		const std::vector<Request*>& GetRequests() const;
		const Request* GetRequest(unsigned int id, uint64_t& next_request) const;

		time_t GetTimelimit() const;
		void   SetTimelimit(time_t seconds);

		bool GetKeepalive() const;
		void SetKeepalive(bool keepalive);

		bool GetFastmode() const;
		void SetFastmode(bool fastmode);

		M::String   GetScript() const;
		void        SetScript(const M::String& script);
		void        SetScript(const char* script);

		bool ParseScript();
		void ParseDocument(const yaml_document_t* document);

		bool AddRequest(const char* url);
};

}

#endif
