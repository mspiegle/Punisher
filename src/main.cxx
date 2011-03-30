/*
 * Punisher
 * by Michael Spiegle
 * 09.03.09
 *
 * main.cxx
 */

#include "Client.hxx"
#include "Logging.hxx"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define APP_VERSION "2.0"

//Commandline stuff
char*        opt_url         = NULL;
bool         opt_help        = false;
bool         opt_quiet       = false;
unsigned int opt_delay       = 1;
char*        opt_script      = NULL;
bool         opt_errors      = false;
int          opt_threads     = 0;
bool         opt_verbose     = false;
bool         opt_version     = false;
bool         opt_fastmode    = false;
int          opt_timelimit   = -1;
bool         opt_keepalive   = false;
int          opt_concurrent  = 64;
unsigned int opt_iterations  = 0;
bool         opt_validation  = false;
unsigned int opt_connections = 0;
char const*  options         = "eqvVkdfc:i:t:s:w:T:";
char const*  version_text    = "Program Version: %s\n";
char const*  help_text       = "Usage: %s [-qvVkf] [-citTw int] "
                               "[-s file] URL\n"
                               "-q:  Enable quiet mode\n"
                               "-v:  Enable verbose mode\n"
                               "-V:  Show program version\n"
                               "-k:  Use keepalives (HTTP/1.1)\n"
                               "-f:  Use Fastmode (Only HTTP/1.0)\n"
                               "-c:  Number of concurrent connections\n"
                               "-i:  Number of iterations to run (requests)\n"
                               "-t:  Number of seconds to run test\n"
                               "-T:  Number of threads to start\n"
                               "-s:  Filename of script to load\n"
                               "-w:  Statistics delay\n"
                               "-e:  Show errors on exit\n"
                               "-d:  Enable validation\n"
                               "\n"
                               "URL can be:\n"
                               "http://hostname:port/path/file.ext\n"
                               "\n";

char*
CheckOptions(int argc, char **argv) {
	int o;

	while(-1 != (o = getopt(argc, argv, options))) {
		switch(o) {
			case 'q': opt_quiet       = true;           break;
			case 'e': opt_errors      = true;           break;
			case 'v': opt_verbose     = true;           break;
			case 'V': opt_version     = true;           break;
			case 'k': opt_keepalive   = true;           break;
			case 'f': opt_fastmode    = true;           break;
			case 'd': opt_validation  = true;           break;
			case 'c': opt_concurrent  = atoi(optarg);   break;
			case 'i': opt_iterations  = atoi(optarg);   break;
			case 't': opt_timelimit   = atoi(optarg);   break;
			case 'T': opt_threads     = atoi(optarg);   break;
			case 'w': opt_delay       = atoi(optarg);   break;
			case 's': opt_script      = strdup(optarg); break;
			default:  return strdup("Invalid Argument!\n");
		}
	}

	if (opt_script == NULL) {
		if ((argv[optind] == NULL) || (strlen(argv[optind]) < 3)) {
			return strdup("You must specify a url or script!\n");
		} else {
			opt_url = strdup(argv[optind]);
		}
	}

	if (opt_fastmode && opt_keepalive) {
		return strdup("You cannot use Fastmode and Keepalive at the same time!\n");
	}

	return NULL;
}

int main(int argc, char **argv) {
	//parse options and check for errors
	char *error = NULL;
	if(NULL != (error = CheckOptions(argc, argv))) {
		printf("There was an error in your arguments!\n");
		printf("%s\n", error);
		printf(help_text, argv[0], argv[0], argv[0]);
		printf("\n");
		printf(version_text, APP_VERSION);
		return EXIT_FAILURE;
	}

	//Logging
	Logging::SetLogLevel(Logging::LEVEL_INFO);
	if (opt_verbose) {
		//Logging::SetTrace(true);
		Logging::SetLogLevel(Logging::LEVEL_DEBUG);
	}

	if (opt_quiet) {
		Logging::SetLogLevel(Logging::LEVEL_CRITICAL);
	}

	//build config
	Punisher::Config* config = new Punisher::Config();
	config->SetConcurrent(opt_concurrent);
	config->SetIterations(opt_iterations);
	config->SetTimelimit(opt_timelimit);
	config->SetKeepalive(opt_keepalive);
	config->SetFastmode(opt_fastmode);
	config->SetDelay(opt_delay);
	config->SetQuiet(opt_quiet);
	config->SetErrors(opt_errors);
	config->SetValidation(opt_validation);
	if (opt_script != NULL) {
		config->SetScript(opt_script);
	} else {
		config->AddRequest(opt_url);
	}
	config->SetThreads(opt_threads);

	//create client and attach config
	Punisher::Client client;
	if (!client.SetConfig(config)) {
		printf("Error: %s\n", client.GetError().ToCString());
		return EXIT_FAILURE;
	}

	client.Start();

	return EXIT_SUCCESS;
}
