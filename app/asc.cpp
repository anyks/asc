/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <env.hpp>
#include <spl.hpp>
#include <ascb.hpp>
#include <toolkit.hpp>
#include <progress.hpp>
#include <collector.hpp>

// Устанавливаем область видимости
using namespace std;
using namespace anyks;
// Активируем пространство имён json
using json = nlohmann::json;

/**
 * version Функция вывода версии приложения
 * @param address адрес приложения
 */
void version(const char * address) noexcept {
	/**
	 * Отлавливаем возможные ошибки
	 */
	try {
		// Позиция в каталоге
		size_t pos = 0;
		/**
		 * Если операционной системой является Linux
		 */
		#ifdef __linux__
			// Название операционной системы
			const char * osname = "Linux";
		/**
		 * Если операционной системой является FreeBSD
		 */
		#elif __FreeBSD__
			// Название операционной системы
			const char * osname = "FreeBSD";
		/**
		 * Если операционной системой является MacOS X
		 */
		#elif __APPLE__ || __MACH__
			// Название операционной системы
			const char * osname = "MacOS X";
		#endif
		// Определяем адрес приложения
		string appname = realpath(address, nullptr);
		// Ищем каталог
		if((pos = appname.rfind("/")) != string::npos) appname = appname.substr(0, pos);
		// Выводим версию приложения
		printf(
			"\r\n%s %s [%s] (built: %s %s)\r\n"
			"target: %s\r\n"
			"installed dir: %s\r\n\r\n*\r\n"
			"* author:   %s\r\n"
			"* telegram: %s\r\n"
			"* email:    %s\r\n"
			"* site:     %s\r\n*\r\n\r\n",
			ANYKS_ASC_NAME,
			ANYKS_ASC_VERSION,
			ANYKS_ASC_DICT_VERSION,
			__DATE__,
			__TIME__,
			osname,
			appname.c_str(),
			ANYKS_ASC_AUTHOR,
			ANYKS_ASC_CONTACT,
			ANYKS_ASC_EMAIL,
			ANYKS_ASC_SITE
		);
	/**
	 * Если происходит ошибка
	 */
	} catch(const exception & error) {
		/**
		 * Если включён режим отладки
		 */
		#if defined(DEBUG_MODE)
			// Выводим сообщение об ошибке
			printf("error: %s\r\n", error.what());
		#endif
	}
}
/**
 * help Функция вывода справки
 */
void help() noexcept {
	// Формируем строку справки
	const string msg = "\r\n\x1B[32m\x1B[1musage:\x1B[0m asc [-V | --version] [-h | --help] "
	"[-alphabet <value> | --alphabet=<value>] [<args>]\r\n\r\n\r\n"
	"\x1B[34m\x1B[1m[METHODS]\x1B[0m\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1minfo\x1B[0m]    method for print information about binary dictionary\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mspell\x1B[0m]   method for spell checking text\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1msplit\x1B[0m]   method for split merged words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mcheck\x1B[0m]   method for checking the existence of a context\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mtrain\x1B[0m]   method for training language model\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mtokens\x1B[0m]  method for getting token type\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mhyphen\x1B[0m]  method of separating hyphenated words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1manalyze\x1B[0m] method for analyzing text for errors\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1merratum\x1B[0m] method for searching misspelled words in text\r\n\r\n"
	"\x1B[34m\x1B[1m[FLAGS]\x1B[0m\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-alm2 | --alm2]                               flag to set ALM type 2\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bloom | --bloom]                             flag allowed to use Bloom filter to check words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-aprune | --aprune]                           flag to pruning language model\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-vprune | --vprune]                           flag to pruning vocabulary\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-stemming | --stemming]                       flag allowing to use stemming for check unknown words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-onlytypos | --onlytypos]                     flag to only correct typos\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-split | --asc-split]                     flag to allow splitting of merged words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-alter | --asc-alter]                     flag that allows you to replace alternative letters in words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-uppers | --asc-uppers]                   flag that allows you to correct the case of letters\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-rsplit | --asc-rsplit]                   flag that allows you to combine words separated by a space\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-hyphen | --asc-hyphen]                   flag to allow splitting of concatenated words with hyphens\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-esplit | --asc-esplit]                   flag to allow splitting of misspelled concatenated words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-skipupp | --asc-skipupp]                 flag to skip uppercase words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-skiphyp | --asc-skiphyp]                 flag to skip hyphenated words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-wordrep | --asc-wordrep]                 flag that allows you to remove duplicate words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-asc-skiplat | --asc-skiplat]                 flag allowing words in the latin alphabet to be skipped\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-allow-unk | --allow-unk]                     flag allowing to unknown word\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-only-good | --only-good]                     flag allowing to consider words from the white list only\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-all-grams | --all-grams]                     flag allowing accounting of all collected n-grams\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-reset-unk | --reset-unk]                     flag to reset the frequency of an unknown word\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-lower-case | --lower-case]                   flag allowing to case-insensitive\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-confidence | --confidence]                   flag arpa file loading without pre-processing the words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-interactive | --interactive]                 flag for run the interactive mode of spell-checking\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-interpolate | --interpolate]                 flag allowing to use interpolation in estimating\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mixed-dicts | --mixed-dicts]                 flag allowing the use of words consisting of mixed dictionaries\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-spell-verbose | --spell-verbose]             flag that allows you to display the result of the spell-checker\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-utokens | --w-bin-utokens]             flag export in binary dictionary of users tokens\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-train-intermed | --train-intermed]           flag to save intermediate data during training\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-train-segments | --train-segments]           flag to performing data segmentation during training\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-allow-apostrophe | --allow-apostrophe]       flag allowing the use of a hyphen as part of the word\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-only-token-words | --only-token-words]       flag that takes into account when assembling N-grams, only those tokens that match words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-tokens-all-disable | --tokens-all-disable]   flag to disable all tokens\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-tokens-all-unknown | --tokens-all-unknown]   flag identifying all tokens as <unk>\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-kneserney-prepares | --kneserney-prepares]   flag allowing need to change grams, after calculating\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-kneserney-modified | --kneserney-modified]   flag allowing modification of the number of already changed minor n-grams\r\n\r\n"
	"\x1B[34m\x1B[1m[ARGS]\x1B[0m\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-ad <value> | --ad=<value>]                                                 number of documents in a text corpus for training\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-cw <value> | --cw=<value>]                                                 number of all words in the text corpus for training\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-log <value> | --log=<value>]                                               address of log file\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-size <value> | --size=<value>]                                             size n-grams of language model\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-alter <value> | --alter=<value>]                                           alternative letters in the format json\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-pilots <value> | --pilots=<value>]                                         an array of words consisting of one letter\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-locale <value> | --locale=<value>]                                         set \x1B[1malm\x1B[0m locale type\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-alphabet <value> | --alphabet=<value>]                                     list letters alphabet (file or value)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-badwords <value> | --badwords=<value>]                                     file address of blacklist words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-goodwords <value> | --goodwords=<value>]                                   file address of whitelist words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-unknown-word <value> | --unknown-word=<value>]                             name of token unknown word\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-embedding <value> | --embedding=<value>]                                   words-embedding parameters in the format json\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-embedding-size <value> | --embedding-size=<value>]                         size words-embedding\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-estimate-count <value> | --estimate-count=<value>]                         maximum number of word variants for analysis\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-word-script <value> | --word-script=<value>]                               address of script python3 - word preprocessing\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-utoken-script <value> | --utoken-script=<value>]                           address of script python3 - custom attribute detector\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-stemming-script <value> | --stemming-script=<value>]                       address of script python3 - words lemmatizer\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-variants-script <value> | --variants-script=<value>]                       address of script python3 - words variants\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-corpus <value> | --corpus=<value>]                                         address of text file training corpus\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-ext <value> | --ext=<value>]                                               extension of files in directory\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-text <value> | --text=<value>]                                             text to be processed as a string\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-threads <value> | --threads=<value>]                                       number of threads for data collection\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-bin <value> | --r-bin=<value>]                                           binary file address ASC of \x1B[1m*.asc\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin <value> | --w-bin=<value>]                                           binary file address ASC of \x1B[1m*.asc\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-lictype <value> | --bin-lictype=<value>]                               license type for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-lictext <value> | --bin-lictext=<value>]                               license text for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-code <value> | --bin-code=<value>]                                     language code for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-name <value> | --bin-name=<value>]                                     dictionary name for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-contacts <value> | --bin-contacts=<value>]                             author contact info for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-author <value> | --bin-author=<value>]                                 author of the dictionary for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-copyright <value> | --bin-copyright=<value>]                           copyright of the dictionary owner for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-aes <value> | --bin-aes=<value>]                                       aes encryption Size \x1B[1m(128, 192, 256) bits\x1B[0m for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-password <value> | --bin-password=<value>]                             encryption password \x1B[1m(if required)\x1B[0m, encryption is performed only when setting a password for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-text <value> | --w-text=<value>]                                         file address for text of \x1B[1m*.txt\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-map <value> | --w-map=<value>]                                           file address for map of \x1B[1m*.map\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-arpa <value> | --w-arpa=<value>]                                         file address for arpa of \x1B[1m*.arpa\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-abbr <value> | --w-abbr=<value>]                                         file address for abbrs of \x1B[1m*.abbr\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-vocab <value> | --w-vocab=<value>]                                       file address for vocab of \x1B[1m*.vocab\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-words <value> | --w-words=<value>]                                       file address for words of \x1B[1m*.txt\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-ngram <value> | --w-ngram=<value>]                                       file address for ngrams of \x1B[1m*.ngrams\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-json <value> | --w-json=<value>]                                         file address for json data of \x1B[1m*.json\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-json <value> | --r-json=<value>]                                         file address for json data of \x1B[1m*.json\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-text <value> | --r-text=<value>]                                         file address for text of \x1B[1m*.txt\x1B[0m or dir path for texts import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-words <value> | --r-words=<value>]                                       file address for words of \x1B[1m*.txt\x1B[0m or dir path for words import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-arpa <value> | --r-arpa=<value>]                                         file address for arpa of \x1B[1m*.arpa\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-abbr <value> | --r-abbr=<value>]                                         file address for abbrs of \x1B[1m*.abbr\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-vocab <value> | --r-vocab=<value>]                                       file address for vocab of \x1B[1m*.vocab\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-alters <value> | --alters=<value>]                                         file address for words with alternate letters of \x1B[1m*.txt\x1B[0m for words import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-upwords <value> | --upwords=<value>]                                       file address for upper case words of \x1B[1m*.txt\x1B[0m or dir path for words import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-abbrs <value> | --abbrs=<value>]                                           file address for abbreviations of \x1B[1m*.txt\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-domain-zones <value> | --domain-zones=<value>]                             file address for domain zones of \x1B[1m*.txt\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mix-restwords <value> | --mix-restwords=<value>]                           file address for restore mixed words of \x1B[1m*.txt\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-abbrs-idw <value> | --w-abbrs-idw=<value>]                               file address for abbrs id of \x1B[1m*.abbr\x1B[0m or dir path for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-badwords-idw <value> | --w-badwords-idw=<value>]                         file address for blacklist id words of \x1B[1m*.txt\x1B[0m or dir path for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-goodwords-idw <value> | --w-goodwords-idw=<value>]                       file address for whitelist id words of \x1B[1m*.txt\x1B[0m or dir path for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-tokens-disable-idw <value> | --w-tokens-disable-idw=<value>]             file address for tokens id to disable of \x1B[1m*.txt\x1B[0m or dir path for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-tokens-unknown-idw <value> | --w-tokens-unknown-idw=<value>]             file address for tokens id to identification as <unk> of \x1B[1m*.txt\x1B[0m or dir path for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-abbrs-idw <value> | --r-abbrs-idw=<value>]                               file address for abbrs id of \x1B[1m*.abbr\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-badwords-idw <value> | --r-badwords-idw=<value>]                         file address for blacklist id words of \x1B[1m*.txt\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-goodwords-idw <value> | --r-goodwords-idw=<value>]                       file address for whitelist id words of \x1B[1m*.txt\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-tokens-disable-idw <value> | --r-tokens-disable-idw=<value>]             file address for tokens id to disable of \x1B[1m*.txt\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-tokens-unknown-idw <value> | --r-tokens-unknown-idw=<value>]             file address for tokens id to identification as <unk> of \x1B[1m*.txt\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-train-intermed-dest <value> | --train-intermed-dest=<value>]               directory path with raw data files for write\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-vprune-oc <value> | --vprune-oc=<value>]                                   \x1B[1moc threshold\x1B[0m of pruning vocabulary\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-vprune-dc <value> | --vprune-dc=<value>]                                   \x1B[1mdc threshold\x1B[0m of pruning vocabulary\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-vprune-wltf <value> | --vprune-wltf=<value>]                               \x1B[1mwltf threshold\x1B[0m of pruning vocabulary\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-aprune-threshold <value> | --aprune-threshold=<value>]                     frequency threshold of pruning language model\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-aprune-max-gram <value> | --aprune-max-gram=<value>]                       maximum size of n-grams pruning\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-utokens <value> | --utokens=<value>]                                       list of custom attributes\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-delta <value> | --delta=<value>]                                           delta size for smoothing by addsmooth algorithm\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-discount <value> | --discount=<value>]                                     discount size for smoothing by cdiscount algorithm\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-debug <value> | --debug=<value>]                                           set debug mode\r\n"
	"  \x1B[1m-\x1B[0m (0 - off | 1 - progress | 2 - console)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-tokens-disable <value1|value2|...> | --tokens-disable=<value1|value2|...>] list of tokens to disable\r\n"
	"  \x1B[1m-\x1B[0m (num | url | abbr | date | time | anum | math | rnum | specl | ...)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-tokens-unknown <value1|value2|...> | --tokens-unknown=<value1|value2|...>] list of tokens for identification as <unk>\r\n"
	"  \x1B[1m-\x1B[0m (num | url | abbr | date | time | anum | math | rnum | specl | ...)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-train-segment-size <value> | --train-segment-size=<value>]                 segment size for training data segmentation\r\n"
	"  \x1B[1m-\x1B[0m (nb | nkb | nMb | nGb), example: --train-segment-size=4096Mb\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-smoothing <value> | --smoothing=<value>]                                   smoothing algorithm for \x1B[1mtrain method\x1B[0m]\r\n"
	"  \x1B[1m-\x1B[0m (goodturing | cdiscount | ndiscount | addsmooth | wittenbell | kneserney | mkneserney)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-method <value> | --method=<value>]                                         method application\r\n"
	"  \x1B[1m-\x1B[0m (train | analyze | hyphen | spell | split | check | erratum | info)\r\n\r\n";
	// Выводим сообщение справки
	printf("%s", msg.c_str());
}
/**
 * print Функция вывода сообщения в консоль
 * @param text     текст для вывода
 * @param filename адрес log файла
 * @param flag     флаг типа сообщения (info, error, warning)
 * @param end      флаг выхода из приложения
 */
void print(const string & text, const char * filename = nullptr, const alphabet_t::log_t flag = alphabet_t::log_t::error, const bool end = true) noexcept {
	// Результат работы функции
	if(!text.empty()){
		// Выполняем логирование
		alphabet_t().log("%s", flag, filename, text.c_str());
		// Выходим из приложения
		if(end) exit(EXIT_FAILURE);
	}
}
/**
 * main Главная функция приложения
 * @param  argc длина массива параметров
 * @param  argv массив параметров
 * @return      код выхода из приложения
 */
int main(int argc, char * argv[]) noexcept {
	// Создаём алфавит
	alphabet_t alphabet;
	// Бинарный файл словаря
	string binDictFile = "";
	// Значение параметра
	const char * value = nullptr;
	// Устанавливаем локаль
	alphabet.setlocale(ALPHABET_LOCALE);
	// Объект отлова переменных
	env_t env(ANYKS_LM_NAME, "text", &alphabet);
	// Выполняем чтение переданных параметров
	env.read((const char **) argv, argc);
	// Если это вывод справки
	if(!env.count() || env.is("help") || env.is("h")) {
		// Выводим справочную информацию
		help();
		// Выходим из приложения
		exit(0);
	// Если версия получена
	} else if(env.is("version") || env.is("V")){
		// Выводим версию приложения
		version(argv[0]);
		// Выходим из приложения
		exit(0);
	}
	// Если локаль передана, устанавливаем новую локаль
	if((value = env.get("locale")) != nullptr) alphabet.setlocale(value);
	// Если JSON файл указан
	if(((value = env.get("r-json")) != nullptr) && fsys_t::isfile(value)){
		// Данные в формате JSON
		string data = "";
		// Выполняем считывание всех строк текста
		fsys_t::rfile(value, [&data](const string & line, const uintmax_t size) noexcept {
			// Если текст получен
			if(!line.empty()) data.append(line);
		});
		// Если данные получены, устанавливаем их
		if(!data.empty()){
			try {
				// Выполняем парсинг JSON конфиг
				env.setJson(json::parse(data));
			// Если возникает ошибка
			} catch(const exception & e) {
				// Выводим сообщение об ошибке
				print(alphabet.format("the JSON config [%s] is broken\r\n", data.c_str()), env.get("log"));
			}
		}
	}
	// Проверяем существует ли бинарный файл
	if(((value = env.get("r-bin")) != nullptr) && fsys_t::isfile(value)){
		// Запоминаем адрес бинарного файла для чтения
		binDictFile = value;
	}
	// Если алфавит снова не найден
	if(binDictFile.empty() && !env.is("alphabet")){
		// Выводим сообщение в консоль
		print("alphabet in not set\r\n", env.get("log"));
	// Продолжаем дальше
	} else {
		// Объявляем прогресс бра
		progress_t pss;
		// Переводим указатель в строку
		string letters = ((value = env.get("alphabet")) != nullptr ? value : "");
		// Проверяем является ли алфавит файлом
		if(fsys_t::isfile(letters)){
			// Получаем адрес файла
			const string filename = letters;
			// Очищаем список букв алфавита
			letters.clear();
			// Выполняем считывание всех строк текста
			fsys_t::rfile(filename, [&letters](const string & line, const uintmax_t size) noexcept {
				// Если текст получен
				if(!line.empty()) letters.append(line);
			});
		}
		// Если алфавит получен
		if(!binDictFile.empty() || !letters.empty()){
			// Режим отладки и общий размер n-граммы
			u_short debug = 0, order = 1;
			// Устанавливаем символы алфавита
			if(!letters.empty()) alphabet.set(alphabet.toLower(letters));
			// Если режим отладки получен
			if((value = env.get("debug")) != nullptr){
				// Устанавливаем тип режима
				debug = stoi(value);
				// Если режим отладки больше указанного, компенсируем
				if(debug > 2) debug = 2;
			}
			// Если общий размер n-граммы получен
			if((value = env.get("size")) != nullptr) order = stoi(value);
			// Если это метод обучения
			if(env.is("method", "train")){
				// Если алгоритм получен
				if(env.is("smoothing") && (string(env.get("smoothing")).compare("-yes-") != 0)){
					// Проверяем правильность введённого алгоритма сглаживания
					if(!env.is("smoothing", "goodturing") &&
					!env.is("smoothing", "cdiscount") &&
					!env.is("smoothing", "ndiscount") &&
					!env.is("smoothing", "addsmooth") &&
					!env.is("smoothing", "wittenbell") &&
					!env.is("smoothing", "kneserney") &&
					!env.is("smoothing", "mkneserney"))
						// Выводим сообщение в консоль
						print(alphabet.format("the algorithm name \"%s\" is bad\r\n", env.get("smoothing")), env.get("log"));
				// Сообщаем, что алгоритм сглаживания не указан
				} else print("algorithm smoothing is not set\r\n", env.get("log"));
			}
			// Если основной метод работы получен
			if(env.is("method") && (string(env.get("method")).compare("-yes-") != 0)){
				// Проверяем правильность введённого основного метода работы
				if(!env.is("method", "check") &&
				!env.is("method", "split") &&
				!env.is("method", "spell") &&
				!env.is("method", "info") &&
				!env.is("method", "train") &&
				!env.is("method", "tokens") &&
				!env.is("method", "hyphen") &&
				!env.is("method", "erratum") &&
				!env.is("method", "analyze"))
					// Выводим сообщение в консоль
					print(alphabet.format("the method name \"%s\" is bad\r\n", env.get("method")), env.get("log"));
			// Сообщаем, что метод не указан
			} else print("toolkit method is not set\r\n", env.get("log"));
			// Если ни один файл для сохранения не передан, выходим
			if(!env.is("w-bin") && !env.is("w-map") && !env.is("w-arpa") &&
			!env.is("w-vocab") && !env.is("w-words") && !env.is("w-ngram") &&
			!env.is("w-text") && !env.is("method", "tokens") &&
			!env.is("method", "check") && !env.is("method", "split") &&
			!env.is("method", "spell") && !env.is("method", "info") &&
			!env.is("method", "train") && !env.is("method", "hyphen") &&
			!env.is("method", "erratum") && !env.is("method", "analyze")){
				// Выводим сообщение и выходим из приложения
				print("file address to save is not specified\r\n", env.get("log"));
			}
			/** Начало работы основных методов **/
			// Создаём токенизатор
			tokenizer_t tokenizer(&alphabet);
			// Устанавливаем адрес файла для логирования
			tokenizer.setLogfile(env.get("log"));
			// Замеряем время начала работы
			auto timeShifting = chrono::system_clock::now();
			// Если апостроф разрешён в токенизаторе, активируем его
			if(env.is("allow-apostrophe")) alphabet.switchAllowApostrophe();
			// Устанавливаем режим отладки, если он передан
			if(debug == 2) tokenizer.setOption(tokenizer_t::options_t::debug);
			// Активируем разрешение проставлять регистры в словах
			if(env.is("asc-uppers")) tokenizer.setOption(tokenizer_t::options_t::uppers);
			// Если файл с буквами для восстановления слов, передан
			if(((value = env.get("mix-restwords")) != nullptr) && fsys_t::isfile(value)){
				/**
				 * Отлавливаем возможные ошибки
				 */
				try {
					// Идентификатор документа
					size_t size = 0;
					// Статус и процентное соотношение
					u_short status = 0, rate = 100;
					// Запоминаем адрес файла
					const string & filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Load mixed letters", "Load mixed letters, is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Список полученных букв
					map <string, string> letters;
					// Выполняем считывание всех строк текста
					fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
						// Если текст получен
						if(!text.empty()){
							// Список пар букв
							vector <wstring> result;
							// Выполняем сплит текста
							alphabet.split(text, "\t", result);
							// Если результат получен
							if(!result.empty() && (result.size() == 2)){
								// Формируем список букв
								letters.emplace(alphabet.convert(result.at(0)), alphabet.convert(result.at(1)));
							}
						}
						// Если отладка включена
						if(debug > 0){
							// Общий полученный размер данных
							size += text.size();
							// Подсчитываем статус выполнения
							status = u_short(size / double(fileSize) * 100.0);
							// Если процентное соотношение изменилось
							if(rate != status){
								// Запоминаем текущее процентное соотношение
								rate = status;
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							}
						}
					});
					// Устанавливаем собранные буквы
					if(!letters.empty()) alphabet.setSubstitutes(letters);
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				/**
				 * Если происходит ошибка
				 */
				} catch(const exception & error) {
					/**
					 * Если включён режим отладки
					 */
					#if defined(DEBUG_MODE)
						// Выводим сообщение об ошибке
						print("error: %s\r\n", error.what());
					#endif
				}
			}
			// Если загрузка списка аббревиатур или доменных зон
			if(env.is("abbrs") || env.is("domain-zones")){
				/**
				 * loadFn Функция загрузки списка доменных зон или аббревиатур
				 * @param filename адрес файла для загрузки
				 * @param type     тип файла для загрузки
				 */
				auto loadFn = [&](const string & filename, const u_short type){
					// Если файл передан
					if(!filename.empty()){
						// Идентификатор документа
						size_t size = 0;
						// Статус и процентное соотношение
						u_short status = 0, rate = 100;
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Устанавливаем название файла
							pss.description(filename);
							// Устанавливаем заголовки прогресс-бара
							switch(type){
								// Если это загрузка доменных зон
								case 1: pss.title("Load domain zones", "Load domain zones, is done"); break;
								// Если это загрузка аббревиатур
								case 2: pss.title("Load abbreviations", "Load abbreviations, is done"); break;
							}
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем считывание всех строк текста
						fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!text.empty()){
								// Устанавливаем собранные текстовые данные
								switch(type){
									// Добавляем доменную зону
									case 1: alphabet.setzone(text); break;
									// Добавляем абреввиатуру
									case 2: tokenizer.addAbbr(text); break;
								}
							}
							// Если отладка включена
							if(debug > 0){
								// Общий полученный размер данных
								size += text.size();
								// Подсчитываем статус выполнения
								status = u_short(size / double(fileSize) * 100.0);
								// Если процентное соотношение изменилось
								if(rate != status){
									// Запоминаем текущее процентное соотношение
									rate = status;
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								}
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
				};
				// Устанавливаем режим считывания файла аббревиатур
				if(((value = env.get("abbrs")) != nullptr) && fsys_t::isfile(value)) loadFn(realpath(value, nullptr), 2);
				// Устанавливаем режим считывания файла доменных зон
				if(((value = env.get("domain-zones")) != nullptr) && fsys_t::isfile(value)) loadFn(realpath(value, nullptr), 1);
			}
			// Если это работа с уже собранным спеллчекером
			if(env.is("method", "check") || env.is("method", "tokens") || env.is("method", "split") || env.is("method", "spell") ||
			env.is("method", "info") || env.is("method", "hyphen") || env.is("method", "erratum") || env.is("method", "analyze")){
				// Создаём обхъект языковой модели
				unique_ptr <alm_t> alm(env.is("alm2") ? new alm2_t(&alphabet, &tokenizer) : new alm1_t(&alphabet, &tokenizer));
				// Устанавливаем адрес файла для логирования
				alm->setLogfile(env.get("log"));
				// Если количество ядер передано
				if(((value = env.get("threads")) != nullptr) && alphabet.isNumber(value)){
					// Устанавливаем количество потоков
					alm->setThreads(stoi(value));
				// Иначе устанавливаем 1 поток
				} else alm->setThreads(1);
				// Устанавливаем режим отладки
				if(debug == 2) alm->setOption(alm_t::options_t::debug);
				// Разрешаем детектировать слова состоящее из смешанных словарей
				if(env.is("mixed-dicts")) alm->setOption(alm_t::options_t::mixdicts);
				// Разрешаем выполнять загрузку содержимого arpa, в том виде, в каком она есть. Без перетокенизации содержимого.
				if(env.is("confidence")) alm->setOption(alm_t::options_t::confidence);
				// Если нужно установить все токены для идентифицирования как <unk>
				if(env.is("tokens-all-unknown")) alm->setAllTokenUnknown();
				// Если нужно установить все токены как не идентифицируемые
				if(env.is("tokens-all-disable")) alm->setAllTokenDisable();
				// Если неизвестное слово получено
				if((value = env.get("unknown-word")) != nullptr) alm->setUnknown(value);
				// Если адрес скрипта получен
				if((value = env.get("word-script")) != nullptr) alm->setWordScript(value);
				// Если нужно установить список токенов которые нужно идентифицировать как <unk>
				if((value = env.get("tokens-unknown")) != nullptr) alm->setTokenUnknown(value);
				// Если нужно установить список не идентифицируемых токенов
				if((value = env.get("tokens-disable")) != nullptr) alm->setTokenDisable(value);
				// Если требуется загрузить файл словаря abbr
				if((value = env.get("r-abbr")) != nullptr){
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Запоминаем адрес файла
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(filename);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read abbr", "Read abbr, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем загрузку файла суффиксов цифровых аббревиатур
						tokenizer.readSuffix(filename, [debug, &pss](const string & filename, const u_short status) noexcept {
							// Если отладка включена, выводим имя файла
							if(debug > 0) pss.description(filename);
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				}
				// Если адрес файла идентификаторов аббревиатур передан
				if((value = env.get("r-abbrs-idw")) != nullptr){
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Список токенов которые нужно идентифицировать как <unk>
						set <size_t> tokens;
						// Параметры индикаторы процесса
						size_t size = 0, status = 0, rate = 0;
						// Запоминаем адрес файла для загрузки
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read abbreviation file", "Read abbreviation file, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем считывание всех слов для списка аббревиатур
						fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!word.empty()){
								// Добавляем в список, полученное слово
								tokens.emplace(stoull(word));
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Общий полученный размер данных
									size += word.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(fileSize) * 100.0);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если список токенов получен, устанавливаем его
						if(!tokens.empty()) tokenizer.setAbbrs(tokens);
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				}
				// Если адрес файла токенов которые нужно идентифицировать как <unk>
				if((value = env.get("r-tokens-unknown-idw")) != nullptr){
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Список токенов которые нужно идентифицировать как <unk>
						set <token_t> tokens;
						// Параметры индикаторы процесса
						size_t size = 0, status = 0, rate = 0;
						// Запоминаем адрес файла для загрузки
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read tokens unknown file", "Read tokens unknown file, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем считывание всех слов для списка токенов которые нужно идентифицировать как <unk>
						fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!word.empty()){
								// Добавляем в список, полученное слово
								tokens.emplace((token_t) stoull(word));
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Общий полученный размер данных
									size += word.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(fileSize) * 100.0);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если список токенов получен, устанавливаем его
						if(!tokens.empty()) alm->setTokensUnknown(tokens);
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				}
				// Если адрес файла не идентифицируемых токенов получен
				if((value = env.get("r-tokens-disable-idw")) != nullptr){
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Список не идентифицируемых токенов
						set <token_t> tokens;
						// Параметры индикаторы процесса
						size_t size = 0, status = 0, rate = 0;
						// Запоминаем адрес файла для загрузки
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read tokens disable file", "Read tokens disable file, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем считывание всех слов для списка не идентифицируемых токенов
						fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!word.empty()){
								// Добавляем в список, полученное слово
								tokens.emplace((token_t) stoull(word));
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Общий полученный размер данных
									size += word.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(fileSize) * 100.0);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если список токенов получен, устанавливаем его
						if(!tokens.empty()) alm->setTokensDisable(tokens);
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				}
				// Если адрес файла чёрного списка получен
				if((value = env.get("badwords")) != nullptr){
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Чёрный список слов
						vector <string> badwords;
						// Параметры индикаторы процесса
						size_t size = 0, status = 0, rate = 0;
						// Запоминаем адрес файла для загрузки
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read badwords file", "Read badwords file, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем считывание всех слов для чёрного списка
						fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!word.empty()){
								// Добавляем в список, полученное слово
								badwords.push_back(word);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Общий полученный размер данных
									size += word.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(fileSize) * 100.0);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если чёрный список получен, устанавливаем его
						if(!badwords.empty()) alm->setBadwords(badwords);
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				}
				// Если адрес файла белого списка получен
				if((value = env.get("goodwords")) != nullptr){
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Белый список слов
						vector <string> goodwords;
						// Параметры индикаторы процесса
						size_t size = 0, status = 0, rate = 0;
						// Запоминаем адрес файла для загрузки
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read goodwords file", "Read goodwords file, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем считывание всех слов для белого списка
						fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
							// Если слово получено
							if(!word.empty()){
								// Добавляем в список, полученное слово
								goodwords.push_back(word);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Общий полученный размер данных
									size += word.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(fileSize) * 100.0);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если белый список получен, устанавливаем его
						if(!goodwords.empty()) alm->setGoodwords(goodwords);
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				}
				// Если адрес файла белого списка идентификаторв получен
				if((value = env.get("r-badwords-idw")) != nullptr){
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Чёрный список слов
						set <size_t> badwords;
						// Параметры индикаторы процесса
						size_t size = 0, status = 0, rate = 0;
						// Запоминаем адрес файла для загрузки
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read badwords file", "Read badwords file, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем считывание всех слов для чёрного списка
						fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!word.empty()){
								// Добавляем в список, полученное слово
								badwords.emplace(stoull(word));
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Общий полученный размер данных
									size += word.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(fileSize) * 100.0);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если чёрный список получен, устанавливаем его
						if(!badwords.empty()) alm->setBadwords(badwords);
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				}
				// Если адрес файла белого списка идентификаторв получен
				if((value = env.get("r-goodwords-idw")) != nullptr){
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Белый список слов
						set <size_t> goodwords;
						// Параметры индикаторы процесса
						size_t size = 0, status = 0, rate = 0;
						// Запоминаем адрес файла для загрузки
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read goodwords file", "Read goodwords file, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем считывание всех слов для белого списка
						fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
							// Если слово получено
							if(!word.empty()){
								// Добавляем в список, полученное слово
								goodwords.emplace(stoull(word));
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Общий полученный размер данных
									size += word.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(fileSize) * 100.0);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если белый список получен, устанавливаем его
						if(!goodwords.empty()) alm->setGoodwords(goodwords);
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				}
				// Если пользовательские токены получены
				if(((value = env.get("utokens")) != nullptr) && (string(value).compare("-yes-") != 0)){
					// Список пользовательских токенов
					vector <wstring> tokens;
					// Выполняем извлечение пользовательских токенов
					alphabet.split(value, "|", tokens);
					// Если список токенов получен
					if(!tokens.empty()){
						// Если адрес скрипта получен
						if((value = env.get("utoken-script")) != nullptr){
							// Устанавливаем адрес скрипта
							alm->setUserTokenScript(value);
							// Переходим по всему списку токенов
							for(auto & item : tokens) alm->setUserToken(alphabet.convert(item));
						}
					}
				}
				// Создаём словарь для работы спеллчекера
				dict_t dict(alm.get(), &alphabet, &tokenizer);
				// Устанавливаем адрес файла для логирования
				dict.setLogfile(env.get("log"));
				// Если количество ядер передано
				if(((value = env.get("threads")) != nullptr) && alphabet.isNumber(value)){
					// Устанавливаем количество потоков
					dict.setThreads(stoi(value));
				// Иначе устанавливаем 1 поток
				} else dict.setThreads(1);
				// Включаем режим отладки
				if(debug == 2) dict.setOption(dict_t::options_t::debug);
				// Использовать в работе фильтр Блума
				if(env.is("bloom")) dict.setOption(dict_t::options_t::bloom);
				// Активировать работу стемминга/лемматизатора
				if(env.is("stemming")) dict.setOption(dict_t::options_t::stemming);
				// Заставляем исправлять только опечатки
				if(env.is("onlytypos")) dict.setOption(dict_t::options_t::onlytypos);
				// Если список пилотных слов передан
				if((value = env.get("pilots")) != nullptr){
					try {
						// Выполняем парсинг пилотных слов
						const json & pilots = json::parse(value);
						// Если пилотные слова получены
						if(!pilots.empty()){
							// Строка содержащее значение
							string text = "";
							// Переходим по всему списку пилотных слов
							for(auto & el : pilots.items()){
								// Получаем значение объекта
								text = el.value();
								// Добавляем пилотное слово
								if(!text.empty()) dict.setPilot(alphabet.convert(text).front());
							}
						}
					// Если возникает ошибка
					} catch(const exception & e) {
						// Выводим сообщение об ошибке
						print(alphabet.format("the JSON pilot words [%s] is broken\r\n", value), env.get("log"));
					}
				}
				// Если альтернативные буквы переданы
				if((value = env.get("alter")) != nullptr){
					try {
						// Выполняем парсинг альтернативные буквы
						const json & alters = json::parse(value);
						// Если альтернативные буквы получены
						if(!alters.empty()){
							// Строка содержащее значение
							string text = "";
							// Ключ и значение объекта
							wchar_t key = 0, value = 0;
							// Переходим по всему списку альтернативных букв
							for(auto & el : alters.items()){
								// Ключ объекта
								text = el.key();
								// Добавляем пилотное слово
								if(!text.empty()){
									// Получаем ключ объекта
									key = alphabet.convert(text).front();
									// Получаем значение объекта
									text = el.value();
									// Добавляем пилотное слово
									if(!text.empty()){
										// Получаем значение объекта
										value = alphabet.convert(text).front();
										// Если оба значения получены
										if((key > 0) && (value > 0)) dict.addAlt(key, value);
									}
								}
							}
						}
					// Если возникает ошибка
					} catch(const exception & e) {
						// Выводим сообщение об ошибке
						print(alphabet.format("the JSON alter letters [%s] is broken\r\n", value), env.get("log"));
					}
				}
				// Если файл с словами начинающихся всегда с заглавной буквы, передан
				if(((value = env.get("upwords")) != nullptr) && fsys_t::isfile(value)){
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Параметры индикаторы процесса
						size_t size = 0, status = 0, rate = 0;
						// Запоминаем адрес файла для загрузки
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read upwords file", "Read upwords file, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем считывание всех альтернативных слов
						fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!word.empty()){
								// Устанавливаем слово, которое всегда должно начинаться с заглавной буквы
								dict.addUWord(word);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Общий полученный размер данных
									size += word.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(fileSize) * 100.0);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				// Если каталог с словами начинающихся всегда с заглавной буквы, передан
				} else if(((value = env.get("upwords")) != nullptr) && fsys_t::isdir(value)) {
					/**
					 * Отлавливаем возможные ошибки
					 */
					try {
						// Параметры индикаторы процесса
						size_t size = 0, status = 0, rate = 0;
						// Запоминаем каталог для загрузки
						const string & path = realpath(value, nullptr);
						// Расширение файлов текстового корпуса
						const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read upwords file", "Read upwords file, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Переходим по всему списку словарей в каталоге
						fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
							// Если отладка включена, выводим название файла
							if(debug > 0) pss.description(filename);
							// Выполняем загрузку файла словаря списка слов
							fsys_t::rfile2(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если слово получено
								if(!word.empty()){
									// Устанавливаем слово, которое всегда должно начинаться с заглавной буквы
									dict.addUWord(word);
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(dirSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					/**
					 * Если происходит ошибка
					 */
					} catch(const exception & error) {
						/**
						 * Если включён режим отладки
						 */
						#if defined(DEBUG_MODE)
							// Выводим сообщение об ошибке
							print("error: %s\r\n", error.what());
						#endif
					}
				}
				// Если файл с альтернативными словами указан
				if(((value = env.get("alters")) != nullptr) && (string(value).compare("-yes-") != 0)){
					// Позиция слова в списоке
					size_t pos = wstring::npos;
					// Параметры индикаторы процесса
					size_t size = 0, status = 0, rate = 0;
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read alters file", "Read alters file, is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем считывание всех альтернативных слов
					fsys_t::rfile(value, [&](const string & line, const uintmax_t fileSize) noexcept {
						// Если текст получен
						if(!line.empty()){
							// Получаем разделитель слов
							pos = line.find("\t");
							// Если разделитель найден
							if(pos != wstring::npos){
								// Добавляем альтернативный вариант
								dict.addAlt(
									alphabet.convert(line.substr(0, pos)),
									alphabet.convert(line.substr(pos + 1))
								);
							}
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Общий полученный размер данных
								size += line.size();
								// Подсчитываем статус выполнения
								status = u_short(size / double(fileSize) * 100.0);
								// Если процентное соотношение изменилось
								if(rate != status){
									// Запоминаем текущее процентное соотношение
									rate = status;
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								}
							}
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				}
				// Если эмбеддинг передан
				if(((value = env.get("embedding")) != nullptr) && (string(value).compare("-yes-") != 0)){
					// Если это файл
					if(fsys_t::isfile(value)){
						// Получаем данные эмбеддинга
						string data = "";
						// Выполняем считывание всех альтернативных слов
						fsys_t::rfile(value, [&data](const string & line, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!line.empty()) data.append(line);
						});
						// Если блок данных получен, формируем эмбеддинг
						if(!data.empty()){
							try {
								// Выполняем парсинг эмбеддинга
								const json & embedding = json::parse(data);
								// Если эмбеддинг получен
								if(!embedding.empty()){
									// Получаем размер эмбеддинга
									const u_int size = (((value = env.get("embedding-size")) != nullptr) ? stoi(value) : embedding.size());
									// Устанавливаем эмбеддинг
									dict.setEmbedding(embedding, size);
								}
							// Если возникает ошибка
							} catch(const exception & e) {
								// Выводим сообщение об ошибке
								print(alphabet.format("the JSON embedding [%s] is broken\r\n", data.c_str()), env.get("log"));
							}
						}
					// Если это не файл
					} else {
						try {
							// Выполняем парсинг эмбеддинга
							const json & embedding = json::parse(value);
							// Если эмбеддинг получен
							if(!embedding.empty()){
								// Получаем размер эмбеддинга
								const u_int size = (((value = env.get("embedding-size")) != nullptr) ? stoi(value) : embedding.size());
								// Устанавливаем эмбеддинг
								dict.setEmbedding(embedding, size);
							}
						// Если возникает ошибка
						} catch(const exception & e) {
							// Выводим сообщение об ошибке
							print(alphabet.format("the JSON embedding [%s] is broken\r\n", value), env.get("log"));
						}
					}
				}
				// Если адрес скрипта лемматизатора, передан
				if((value = env.get("stemming-script")) != nullptr) dict.setLScript(value);
				// Если адрес скрипта подбора вариантов, передан
				if((value = env.get("variants-script")) != nullptr) dict.setVScript(value);
				// Создаём объект спеллчекера
				spell_t spell(&dict, alm.get(), &alphabet, &tokenizer);
				// Устанавливаем адрес файла для логирования
				spell.setLogfile(env.get("log"));
				// Если нужно использовать бинарный контейнер
				if(!binDictFile.empty() && env.is("r-bin")){
					// Объект бинарного контейнера
					ascb_t ascb(binDictFile, ((value = env.get("bin-password")) != nullptr ? value : ""), env.get("log"));
					// Устанавливаем словарь
					ascb.setDict(&dict);
					// Устанавливаем режим отладки
					ascb.setDebug(debug);
					// Устанавливаем языковую модель
					ascb.setAlm(alm.get());
					// Устанавливаем прогресс-бар
					ascb.setProgress(&pss);
					// Устанавливаем алфавит
					ascb.setAlphabet(&alphabet);
					// Устанавливаем токенизатор
					ascb.setTokenizer(&tokenizer);
					// Устанавливаем тип языковой модели
					ascb.setALMvType(env.is("alm2"));
					// Выполняем инициализацию контейнера
					ascb.init();
					// Если это запрос информации о словаре
					if(env.is("method", "info")){
						// Выводим информацию о контейнере
						ascb.info();
						// Если режим отладки включён
						if(debug > 0){
							// Получаем диапазон времени
							auto dimension = pss.dimension(chrono::duration_cast <chrono::seconds> (chrono::system_clock::now() - timeShifting).count());
							// Выводим результат
							print(alphabet.format("work time shifting: %lld %s\r\n", dimension.first, dimension.second.c_str()), env.get("log"), alphabet_t::log_t::info);
						// Выходим из приложения
						} else exit(0);
					// Выполняем чтение контейнера
					} else ascb.read();
				// Если требуется работа с текстовыми файлами
				} else {
					// Если требуется загрузить файл словаря vocab
					if(((value = env.get("r-vocab")) != nullptr) && fsys_t::isfile(value)){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Запоминаем адрес файла
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем название файла
								pss.description(filename);
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read vocab file", "Read vocab file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем загрузку файла словаря vocab
							dict.readVocab(filename, [debug, &pss](const u_short status) noexcept {
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если требуется загрузить arpa
					if(((value = env.get("r-arpa")) != nullptr) && fsys_t::isfile(value)){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Запоминаем адрес файла
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем название файла
								pss.description(filename);
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read arpa file", "Read arpa file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем чтение arpa
							alm->read(filename, [debug, &pss](const u_short status) noexcept {
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Train dictionary", "Train dictionary, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем обучение
							dict.train([debug, &pss](const u_short status) noexcept {
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					// Если arpa файл не указан
					} else print("language model file address is empty\r\n", env.get("log"));
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Loading Bloom filter", "Loading Bloom filter, is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку фильтра Блума
					dict.bloom([debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Loading stemming", "Loading stemming, is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку стемминга
					dict.stemming([debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
					// Если переданы данные о количество слов в словаре или количестве документов
					if(env.is("ad") || env.is("cw")) dict.setAdCw(env.is("cw") ? 1 : stoull(env.get("cw")), env.is("ad") ? 1 : stoull(env.get("ad")));
				}
				// Если это методы работы с спеллчекером
				if(env.is("method", "check") || env.is("method", "tokens") || env.is("method", "split") ||
				env.is("method", "spell") || env.is("method", "hyphen") || env.is("method", "erratum") || env.is("method", "analyze")){
					// Активируем разрешение использовать сплиты
					size_t options = 0;
					// Включаем режим отладки для спеллчекера
					if(debug == 2) options = spell.setOption(spell_t::options_t::debug, options);
					// Включаем режим разрешения выполнять сплитов текста
					if(env.is("asc-split")) options = spell.setOption(spell_t::options_t::split, options);
					// Активируем разрешение на исправление слов с альтернативными буквами
					if(env.is("asc-alter")) options = spell.setOption(spell_t::options_t::alter, options);
					// Активируем разрешение использовать сплиты с ошибками
					if(env.is("asc-esplit")) options = spell.setOption(spell_t::options_t::esplit, options);
					// Активируем разрешение на удаление сплитов в словах
					if(env.is("asc-rsplit")) options = spell.setOption(spell_t::options_t::rsplit, options);
					// Активируем разрешение проставлять регистры в словах
					if(env.is("asc-uppers")) options = spell.setOption(spell_t::options_t::uppers, options);
					// Активируем разрешение выполнять сплиты по дефисам
					if(env.is("asc-hyphen")) options = spell.setOption(spell_t::options_t::hyphen, options);
					// Разрешаем пропускать слова в верхнем регистре
					if(env.is("asc-skipupp")) options = spell.setOption(spell_t::options_t::skipupp, options);
					// Разрешаем пропускать слова с латинскими символами
					if(env.is("asc-skiplat")) options = spell.setOption(spell_t::options_t::skiplat, options);
					// Разрешаем пропускать слова с дефисами
					if(env.is("asc-skiphyp")) options = spell.setOption(spell_t::options_t::skiphyp, options);
					// Разрешаем удалять повторяющиеся одинаковые слова
					if(env.is("asc-wordrep")) options = spell.setOption(spell_t::options_t::wordrep, options);
					// Устанавливаем количество вариантов для обработки
					if(((value = env.get("estimate-count")) != nullptr) && alphabet.isNumber(value)) dict.setNSWLibCount(stoull(value));
					// Если это метод проверки на ошибки
					if(env.is("method", "erratum")){
						// Список слов с ошибками
						vector <wstring> errors;
						// Если текст передан
						if(((value = env.get("text")) != nullptr) && (string(value).compare("-yes-") != 0)){
							// Выполняем поиск слов с ошибками
							spell.erratum(alphabet.convert(value), options, errors);
							// Если список слов с ошибками получен
							if(!errors.empty()){
								// Выводим все найденные ошибки
								for(auto & error : errors) printf("%ls\r\n", error.c_str());
								// Выводим разделитель
								if(debug > 0) cout << endl;
							}
						// Если адрес текстового файла или каталог передан
						} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value)) && ((debug == 0) || (env.get("log") != nullptr))) {
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Идентификатор документа
								size_t size = 0;
								// Статус и процентное соотношение
								u_short status = 0, rate = 100;
								// Запоминаем адрес файла
								const string & path = realpath(value, nullptr);
								// Расширение файлов текстового корпуса
								const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(path);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Search for errors in the text", "Search for errors in the text, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Замеряем время завершения работы анализатора
								auto end = chrono::system_clock::now();
								// Замеряем время работы анализатора
								auto start = chrono::system_clock::now();
								// Если это каталог
								if(fsys_t::isdir(path)){
									// Переходим по всему списку файлов в каталоге
									fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
										// Устанавливаем название файла
										if(debug > 0) pss.description(filename);
										// Выполняем считывание всех строк текста
										fsys_t::rfile2(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
											// Если текст получен
											if(!text.empty()){
												// Замеряем время работы анализатора
												start = chrono::system_clock::now();
												// Выполняем поиск слов с ошибками
												spell.erratum(alphabet.convert(text), options, errors);
												// Замеряем время завершения работы анализатора
												end = chrono::system_clock::now();
												// Если список слов с ошибками получен
												if(!errors.empty()){
													// Выводим разделитель
													print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
													// Отображаем результат
													print(alphabet.format("Text: %s", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
													// Выводим все найденные ошибки
													for(auto & error : errors) print(alphabet.format("Unknown: %ls", error.c_str()), env.get("log"), alphabet_t::log_t::null, false);
													// Выводим сообщение о затраченном времени
													print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
													// Выводим разделитель
													print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
												}
											}
											// Если отладка включена
											if(debug > 0){
												// Общий полученный размер данных
												size += text.size();
												// Подсчитываем статус выполнения
												status = u_short(size / double(dirSize) * 100.0);
												// Если процентное соотношение изменилось
												if(rate != status){
													// Запоминаем текущее процентное соотношение
													rate = status;
													// Отображаем ход процесса
													switch(debug){
														case 1: pss.update(status); break;
														case 2: pss.status(status); break;
													}
												}
											}
										});
									});
								// Если это не каталог а файл
								} else {
									// Выполняем считывание всех строк текста
									fsys_t::rfile(path, [&](const string & text, const uintmax_t fileSize) noexcept {
										// Если текст получен
										if(!text.empty()){
											// Замеряем время работы анализатора
											start = chrono::system_clock::now();
											// Выполняем поиск слов с ошибками
											spell.erratum(alphabet.convert(text), options, errors);
											// Замеряем время завершения работы анализатора
											end = chrono::system_clock::now();
											// Если список слов с ошибками получен
											if(!errors.empty()){
												// Выводим разделитель
												print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
												// Отображаем результат
												print(alphabet.format("Text: %s", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим все найденные ошибки
												for(auto & error : errors) print(alphabet.format("Unknown: %ls", error.c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим сообщение о затраченном времени
												print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим разделитель
												print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
											}
										}
										// Если отладка включена
										if(debug > 0){
											// Общий полученный размер данных
											size += text.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(fileSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									});
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						// Переводим работу в интерактивный режим
						} else if(env.is("interactive")) {
							// Текст для исправления
							wstring text = L"";
							// Буфер данных для ввода текста
							string buffer = "";
							// Список слов с ошибками
							vector <wstring> errors;
							// Информационные сообщения
							const string info1 = "To exit, enter", info2 = "Enter text for checking";
							// Выполняем очистку консоли
							printf("\033c");
							// Выводим сообщение
							printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
							// Выполняем чтение данных в буфер
							while(true){
								// Считываем данные в буфер
								getline(cin, buffer);
								// Если данные существуют
								if(!buffer.empty()){
									// Если это выход тогда выходим
									if(buffer.compare("end") == 0){
										// Выполняем очистку консоли
										printf("\033c");
										// Выводим прощание
										printf("\r\n\x1B[34m\x1B[1mGoodbye!\x1B[0m\r\n\r\n");
										// Выходим из приложения
										break;
									}
									// Получаем текст для исправления
									text = alphabet.convert(buffer);
									// Выполняем поиск слов с ошибками
									spell.erratum(text, options, errors);
									// Выполняем очистку консоли
									printf("\033c");
									// Выводим сообщение
									printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
									// Выводим исходный текст
									printf("\r\n\x1B[1mOriginal text:\x1B[0m\r\n%s\r\n\r\n", buffer.c_str());
									// Выводим результат
									printf("\x1B[1m%s:\x1B[0m\r\n", "Is not correct");
									// Если список слов с ошибками получен
									if(!errors.empty()){
										// Выводим все найденные ошибки
										for(auto & error : errors) printf("%ls\r\n", error.c_str());
									}
								}
							}
							// Выходим из приложения
							exit(0);
						// Сообщаем, что текст не указан
						} else print("text is empty or wrong params\r\n", env.get("log"));
					// Если это метод определения токена слова
					} else if(env.is("method", "tokens")){
						// Если текст передан
						if(((value = env.get("text")) != nullptr) && (string(value).compare("-yes-") != 0)){
							// Выполняем получение токена слова
							const size_t idw = alm->getIdw(alphabet.convert(value));
							// Извлекаем данные слова
							const auto & token = alm->word(idw);
							// Если - это не токен, значит слово
							if((token.front() == L'<') && (token.back() == L'>'))
								// Выводим тип полученного токена
								printf("%s\r\n", token.str().c_str());
							// Сообщаем, что - это обычное слово
							else printf("%s\r\n", "<word>");
							// Выводим разделитель
							if(debug > 0) cout << endl;
						// Если адрес текстового файла или каталог передан
						} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value)) && ((debug == 0) || (env.get("log") != nullptr))) {
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Идентификатор документа
								size_t size = 0;
								// Статус и процентное соотношение
								u_short status = 0, rate = 100;
								// Запоминаем адрес файла
								const string & path = realpath(value, nullptr);
								// Расширение файлов текстового корпуса
								const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(path);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Get token by word in the text", "Get token by word in the text, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Если это каталог
								if(fsys_t::isdir(path)){
									// Переходим по всему списку файлов в каталоге
									fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
										// Устанавливаем название файла
										if(debug > 0) pss.description(filename);
										// Выполняем считывание всех строк текста
										fsys_t::rfile2(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
											// Если слово получено
											if(!word.empty()){
												// Выводим разделитель
												print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
												// Выполняем получение токена слова
												const size_t idw = alm->getIdw(alphabet.convert(word));
												// Извлекаем данные слова
												const auto & token = alm->word(idw);
												// Отображаем результат
												print(alphabet.format("Word: %s", word.c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Если - это не токен, значит слово
												if((token.front() == L'<') && (token.back() == L'>'))
													// Выводим тип полученного токена
													print(alphabet.format("Token: %s", token.str().c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Сообщаем, что - это обычное слово
												else print("Token: <word>", env.get("log"), alphabet_t::log_t::null, false);
												// Выводим разделитель
												print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
											}
											// Если отладка включена
											if(debug > 0){
												// Общий полученный размер данных
												size += word.size();
												// Подсчитываем статус выполнения
												status = u_short(size / double(dirSize) * 100.0);
												// Если процентное соотношение изменилось
												if(rate != status){
													// Запоминаем текущее процентное соотношение
													rate = status;
													// Отображаем ход процесса
													switch(debug){
														case 1: pss.update(status); break;
														case 2: pss.status(status); break;
													}
												}
											}
										});
									});
								// Если это не каталог а файл
								} else {
									// Выполняем считывание всех строк текста
									fsys_t::rfile(path, [&](const string & word, const uintmax_t fileSize) noexcept {
										// Если слово получено
										if(!word.empty()){
											// Выводим разделитель
											print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
											// Выполняем получение токена слова
											const size_t idw = alm->getIdw(alphabet.convert(word));
											// Извлекаем данные слова
											const auto & token = alm->word(idw);
											// Отображаем результат
											print(alphabet.format("Word: %s", word.c_str()), env.get("log"), alphabet_t::log_t::null, false);
											// Если - это не токен, значит слово
											if((token.front() == L'<') && (token.back() == L'>'))
												// Выводим тип полученного токена
												print(alphabet.format("Token: %s", token.str().c_str()), env.get("log"), alphabet_t::log_t::null, false);
											// Сообщаем, что - это обычное слово
											else print("Token: <word>", env.get("log"), alphabet_t::log_t::null, false);
											// Выводим разделитель
											print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
										}
										// Если отладка включена
										if(debug > 0){
											// Общий полученный размер данных
											size += word.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(fileSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									});
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						// Переводим работу в интерактивный режим
						} else if(env.is("interactive")) {
							// Буфер данных для ввода текста
							string buffer = "";
							// Идентификатор слова
							size_t idw = idw_t::NIDW;
							// Информационные сообщения
							const string info1 = "To exit, enter", info2 = "Enter text for checking";
							// Выполняем очистку консоли
							printf("\033c");
							// Выводим сообщение
							printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
							// Выполняем чтение данных в буфер
							while(true){
								// Считываем данные в буфер
								getline(cin, buffer);
								// Если данные существуют
								if(!buffer.empty()){
									// Если это выход тогда выходим
									if(buffer.compare("end") == 0){
										// Выполняем очистку консоли
										printf("\033c");
										// Выводим прощание
										printf("\r\n\x1B[34m\x1B[1mGoodbye!\x1B[0m\r\n\r\n");
										// Выходим из приложения
										break;
									}
									// Выполняем получение токена слова
									idw = alm->getIdw(alphabet.convert(buffer));
									// Извлекаем данные слова
									const auto & token = alm->word(idw);
									// Выполняем очистку консоли
									printf("\033c");
									// Выводим сообщение
									printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
									// Выводим исходный текст
									printf("\r\n\x1B[1mOriginal word:\x1B[0m %s\r\n\r\n", buffer.c_str());
									// Если - это не токен, значит слово
									if((token.front() == L'<') && (token.back() == L'>'))
										// Выводим тип полученного токена
										printf("\x1B[1mToken word:\x1B[0m %s\r\n\r\n", token.str().c_str());
									// Сообщаем, что - это обычное слово
									else printf("\x1B[1mToken word:\x1B[0m %s\r\n\r\n", "<word>");
								}
							}
							// Выходим из приложения
							exit(0);
						// Сообщаем, что текст не указан
						} else print("text is empty or wrong params\r\n", env.get("log"));
					// Если нужно проверить существование слова
					} else if(env.is("method", "check")) {
						// Если текст передан
						if(((value = env.get("text")) != nullptr) && (string(value).compare("-yes-") != 0)){
							// Выполняем проверку существования слова
							if(spell.check(alphabet.convert(value)))
								// Выводим сообщение, что слово существует
								printf("%s\r\n", "YES");
							// Выводим сообщение, что слово не существует
							else printf("%s\r\n", "NO");
							// Выводим разделитель
							if(debug > 0) cout << endl;
						// Если адрес текстового файла или каталог передан
						} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value)) && ((debug == 0) || (env.get("log") != nullptr))) {
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Идентификатор документа
								size_t size = 0;
								// Статус и процентное соотношение
								u_short status = 0, rate = 100;
								// Запоминаем адрес файла
								const string & path = realpath(value, nullptr);
								// Расширение файлов текстового корпуса
								const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(path);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Check word in the text", "Check word in the text, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Если это каталог
								if(fsys_t::isdir(path)){
									// Переходим по всему списку файлов в каталоге
									fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
										// Устанавливаем название файла
										if(debug > 0) pss.description(filename);
										// Выполняем считывание всех строк текста
										fsys_t::rfile2(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
											// Если слово получено
											if(!word.empty()){
												// Выводим разделитель
												print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
												// Отображаем результат
												print(alphabet.format("Word: %s", word.c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим все найденные ошибки
												print(alphabet.format("Is exist: %s", (spell.check(alphabet.convert(word)) ? "YES" : "NO")), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим разделитель
												print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
											}
											// Если отладка включена
											if(debug > 0){
												// Общий полученный размер данных
												size += word.size();
												// Подсчитываем статус выполнения
												status = u_short(size / double(dirSize) * 100.0);
												// Если процентное соотношение изменилось
												if(rate != status){
													// Запоминаем текущее процентное соотношение
													rate = status;
													// Отображаем ход процесса
													switch(debug){
														case 1: pss.update(status); break;
														case 2: pss.status(status); break;
													}
												}
											}
										});
									});
								// Если это не каталог а файл
								} else {
									// Выполняем считывание всех строк текста
									fsys_t::rfile(path, [&](const string & word, const uintmax_t fileSize) noexcept {
										// Если слово получено
										if(!word.empty()){
											// Выводим разделитель
											print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
											// Отображаем результат
											print(alphabet.format("Word: %s", word.c_str()), env.get("log"), alphabet_t::log_t::null, false);
											// Выводим все найденные ошибки
											print(alphabet.format("Is exist: %s", (spell.check(alphabet.convert(word)) ? "YES" : "NO")), env.get("log"), alphabet_t::log_t::null, false);
											// Выводим разделитель
											print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
										}
										// Если отладка включена
										if(debug > 0){
											// Общий полученный размер данных
											size += word.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(fileSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									});
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						// Переводим работу в интерактивный режим
						} else if(env.is("interactive")) {
							// Текст для исправления
							wstring text = L"";
							// Буфер данных для ввода текста
							string buffer = "";
							// Результат првоерки
							bool isCheck = false;
							// Информационные сообщения
							const string info1 = "To exit, enter", info2 = "Enter text for checking";
							// Выполняем очистку консоли
							printf("\033c");
							// Выводим сообщение
							printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
							// Выполняем чтение данных в буфер
							while(true){
								// Считываем данные в буфер
								getline(cin, buffer);
								// Если данные существуют
								if(!buffer.empty()){
									// Если это выход тогда выходим
									if(buffer.compare("end") == 0){
										// Выполняем очистку консоли
										printf("\033c");
										// Выводим прощание
										printf("\r\n\x1B[34m\x1B[1mGoodbye!\x1B[0m\r\n\r\n");
										// Выходим из приложения
										break;
									}
									// Получаем текст для исправления
									text = alphabet.convert(buffer);
									// Выполняем проверку существования слова
									isCheck = spell.check(text);
									// Выполняем очистку консоли
									printf("\033c");
									// Выводим сообщение
									printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
									// Выводим исходный текст
									printf("\r\n\x1B[1mOriginal word:\x1B[0m %s\r\n\r\n", buffer.c_str());
									// Выводим сообщение, что слово существует
									if(isCheck) printf("\x1B[1mChecking word:\x1B[0m %s\r\n\r\n", "YES");
									// Выводим сообщение, что слово не существует
									else printf("\x1B[1mChecking word:\x1B[0m %s\r\n\r\n", "NO");
								}
							}
							// Выходим из приложения
							exit(0);
						// Сообщаем, что текст не указан
						} else print("text is empty or wrong params\r\n", env.get("log"));
					// Если нужно выполнять сплит слова
					} else if(env.is("method", "split")) {
						// Список проспличенных слов
						vector <wstring> words;
						// Если текст передан
						if(((value = env.get("text")) != nullptr) && (string(value).compare("-yes-") != 0)){
							// Выполняем сплит слова
							spell.split(alphabet.convert(value), options, words);
							// Если список слов с ошибками получен
							if(!words.empty())
								// Выполняем восстановление контекста
								printf("%ls\r\n", tokenizer.restore(words).c_str());
							// Выводим текст как передан
							else printf("%s\r\n", value);
							// Выводим разделитель
							if(debug > 0) cout << endl;
						// Если адрес текстового файла или каталог передан
						} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value)) && ((debug == 0) || (env.get("log") != nullptr))) {
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Идентификатор документа
								size_t size = 0;
								// Статус и процентное соотношение
								u_short status = 0, rate = 100;
								// Запоминаем адрес файла
								const string & path = realpath(value, nullptr);
								// Расширение файлов текстового корпуса
								const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(path);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Split words in the text", "Split words in the text, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Замеряем время завершения работы анализатора
								auto end = chrono::system_clock::now();
								// Замеряем время работы анализатора
								auto start = chrono::system_clock::now();
								// Если это каталог
								if(fsys_t::isdir(path)){
									// Переходим по всему списку файлов в каталоге
									fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
										// Устанавливаем название файла
										if(debug > 0) pss.description(filename);
										// Выполняем считывание всех строк текста
										fsys_t::rfile2(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
											// Если текст получен
											if(!text.empty()){
												// Замеряем время работы анализатора
												start = chrono::system_clock::now();
												// Выполняем сплит слова
												spell.split(alphabet.convert(text), options, words);
												// Замеряем время завершения работы анализатора
												end = chrono::system_clock::now();
												// Выводим разделитель
												print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
												// Отображаем результат
												print(alphabet.format("Text: %s", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим получившийся текст
												print(alphabet.format("Split: %ls", (!words.empty() ? tokenizer.restore(words).c_str() : alphabet.convert(text).c_str())), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим сообщение о затраченном времени
												print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим разделитель
												print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
											}
											// Если отладка включена
											if(debug > 0){
												// Общий полученный размер данных
												size += text.size();
												// Подсчитываем статус выполнения
												status = u_short(size / double(dirSize) * 100.0);
												// Если процентное соотношение изменилось
												if(rate != status){
													// Запоминаем текущее процентное соотношение
													rate = status;
													// Отображаем ход процесса
													switch(debug){
														case 1: pss.update(status); break;
														case 2: pss.status(status); break;
													}
												}
											}
										});
									});
								// Если это не каталог а файл
								} else {
									// Выполняем считывание всех строк текста
									fsys_t::rfile(path, [&](const string & text, const uintmax_t fileSize) noexcept {
										// Если текст получен
										if(!text.empty()){
											// Замеряем время работы анализатора
											start = chrono::system_clock::now();
											// Выполняем сплит слова
											spell.split(alphabet.convert(text), options, words);
											// Замеряем время завершения работы анализатора
											end = chrono::system_clock::now();
											// Выводим разделитель
											print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
											// Отображаем результат
											print(alphabet.format("Text: %s", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
											// Выводим получившийся текст
											print(alphabet.format("Split: %ls", (!words.empty() ? tokenizer.restore(words).c_str() : alphabet.convert(text).c_str())), env.get("log"), alphabet_t::log_t::null, false);
											// Выводим сообщение о затраченном времени
											print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
											// Выводим разделитель
											print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
										}
										// Если отладка включена
										if(debug > 0){
											// Общий полученный размер данных
											size += text.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(fileSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									});
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						// Переводим работу в интерактивный режим
						} else if(env.is("interactive")) {
							// Текст для исправления
							wstring text = L"";
							// Буфер данных для ввода текста
							string buffer = "";
							// Список проспличенных слов
							vector <wstring> words;
							// Информационные сообщения
							const string info1 = "To exit, enter", info2 = "Enter text for splited";
							// Выполняем очистку консоли
							printf("\033c");
							// Выводим сообщение
							printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
							// Выполняем чтение данных в буфер
							while(true){
								// Считываем данные в буфер
								getline(cin, buffer);
								// Если данные существуют
								if(!buffer.empty()){
									// Если это выход тогда выходим
									if(buffer.compare("end") == 0){
										// Выполняем очистку консоли
										printf("\033c");
										// Выводим прощание
										printf("\r\n\x1B[34m\x1B[1mGoodbye!\x1B[0m\r\n\r\n");
										// Выходим из приложения
										break;
									}
									// Получаем текст для исправления
									text = alphabet.convert(buffer);
									// Выполняем сплит слова
									spell.split(text, options, words);
									// Выполняем очистку консоли
									printf("\033c");
									// Выводим сообщение
									printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
									// Выводим исходный текст
									printf("\r\n\x1B[1mOriginal text:\x1B[0m\r\n%s\r\n\r\n", buffer.c_str());
									// Если список слов с ошибками получен
									if(!words.empty())
										// Выполняем восстановление контекста
										printf("\x1B[1mSplited text:\x1B[0m\r\n%ls\r\n\r\n", tokenizer.restore(words).c_str());
									// Выводим текст как передан
									else printf("\x1B[1mSplited text:\x1B[0m\r\n%ls\r\n\r\n", text.c_str());
								}
							}
							// Выходим из приложения
							exit(0);
						// Сообщаем, что текст не указан
						} else print("text is empty or wrong params\r\n", env.get("log"));
					// Если нужно выполнить исправление ошибок в тексте
					} else if(env.is("method", "spell")) {
						// Информационные собранные данные исправлений
						vector <vector <pair <wstring, wstring>>> info;
						// Если текст передан
						if(((value = env.get("text")) != nullptr) && (string(value).compare("-yes-") != 0)){
							// Получаем текст для исправления
							wstring text = alphabet.convert(value);
							// Выполняем исправление опечаток
							spell.spell(text, options, (env.is("spell-verbose") ? &info : nullptr));
							// Выводим получившийся результат
							printf("%ls\r\n", text.c_str());
							// Выводим разделитель
							if(debug > 0) cout << endl;
							// Если информационный блок получен
							if(!info.empty()){
								// Переходим по всему блоку информации
								for(auto & item : info){
									// Переходим по всему списку вариантов
									for(auto & value : item){
										// Если варианты разные
										if(value.first.compare(value.second) != 0){
											// Если - это не существующий контекст
											if(value.second.compare(NOTFOUND) == 0)
												// Выводим тело в консоль
												printf("[%ls] to [is not correct]\r\n", value.first.c_str());
											// Выводим тело в консоль
											else printf("[%ls] to [%ls]\r\n", value.first.c_str(), value.second.c_str());
										}
									}
									// Выводим разделитель
									if(debug > 0) cout << endl;
								}
							}
						// Если адрес текстового файла или каталог передан
						} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value))) {
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Идентификатор документа
								size_t size = 0;
								// Статус и процентное соотношение
								u_short status = 0, rate = 100;
								// Запоминаем адрес файла
								const string & path = realpath(value, nullptr);
								// Расширение файлов текстового корпуса
								const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
								// Запоминаем файл для записи данных
								const string writefile = ((value = env.get("w-text")) != nullptr ? value : "");
								// Если отладка включена, выводим индикатор загрузки
								if(!env.is("spell-verbose") && (debug > 0)){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(path);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Spell checking in the text", "Spell checking in the text, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Открываем файл на запись
								ofstream file(writefile, ios::binary);
								// Замеряем время завершения работы анализатора
								auto end = chrono::system_clock::now();
								// Замеряем время работы анализатора
								auto start = chrono::system_clock::now();
								// Если это каталог
								if(fsys_t::isdir(path)){
									// Переходим по всему списку файлов в каталоге
									fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
										// Устанавливаем название файла
										if(!env.is("spell-verbose") && (debug > 0)) pss.description(filename);
										// Выполняем считывание всех строк текста
										fsys_t::rfile2(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
											// Если текст получен
											if(!text.empty()){
												// Получаем текст для исправления
												wstring tmp = alphabet.convert(text);
												// Замеряем время работы анализатора
												start = chrono::system_clock::now();
												// Выполняем исправление опечаток
												spell.spell(tmp, options, (env.is("spell-verbose") ? &info : nullptr));
												// Замеряем время завершения работы анализатора
												end = chrono::system_clock::now();
												// Если файл открыт, выполняем запись в файл результата
												if(file.is_open()){
													// Создаём текст для записи
													const string & text = alphabet.format("%ls\r\n", tmp.c_str());
													// Выполняем запись данных в файл
													file.write(text.data(), text.size());
												}
												// Если нужно выводить информацию статистики
												if(env.is("spell-verbose") && !info.empty()){
													// Выводим разделитель
													print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
													// Отображаем входящий текст
													print(alphabet.format("Text: %s\r\n", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
													// Отображаем результат
													print(alphabet.format("Result: %ls\r\n", tmp.c_str()), env.get("log"), alphabet_t::log_t::null, false);
													// Отображаем загоовок списка исправлений
													print("The analytics:", env.get("log"), alphabet_t::log_t::null, false);
													// Переходим по всему блоку информации
													for(auto & item : info){
														// Переходим по всему списку вариантов
														for(auto & value : item){
															// Если варианты разные
															if(value.first.compare(value.second) != 0){
																// Если - это не существующий контекст
																if(value.second.compare(NOTFOUND) == 0){
																	// Выводим результат в консоль
																	print(alphabet.format("Info: [%ls] to [is not correct]", value.first.c_str()), env.get("log"), alphabet_t::log_t::null, false);
																// Выводим результат в консоль
																} else print(alphabet.format("Replace: [%ls] to [%ls]", value.first.c_str(), value.second.c_str()), env.get("log"), alphabet_t::log_t::null, false);
															// Выводим сообщение, что всё хорошо
															} else print(alphabet.format("Info: [%ls] to [correct]", value.first.c_str()), env.get("log"), alphabet_t::log_t::null, false);
														}
														// Выводим разделитель
														print("\r\n", env.get("log"), alphabet_t::log_t::null, false);
													}
													// Выводим сообщение о затраченном времени
													print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
													// Выводим разделитель
													print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
												}
											}
											// Если отладка включена
											if(!env.is("spell-verbose") && (debug > 0)){
												// Общий полученный размер данных
												size += text.size();
												// Подсчитываем статус выполнения
												status = u_short(size / double(dirSize) * 100.0);
												// Если процентное соотношение изменилось
												if(rate != status){
													// Запоминаем текущее процентное соотношение
													rate = status;
													// Отображаем ход процесса
													switch(debug){
														case 1: pss.update(status); break;
														case 2: pss.status(status); break;
													}
												}
											}
										});
									});
								// Если это не каталог а файл
								} else {
									// Выполняем считывание всех строк текста
									fsys_t::rfile(path, [&](const string & text, const uintmax_t fileSize) noexcept {
										// Если текст получен
										if(!text.empty()){
											// Получаем текст для исправления
											wstring tmp = alphabet.convert(text);
											// Замеряем время работы анализатора
											start = chrono::system_clock::now();
											// Выполняем исправление опечаток
											spell.spell(tmp, options, (env.is("spell-verbose") ? &info : nullptr));
											// Замеряем время завершения работы анализатора
											end = chrono::system_clock::now();
											// Если файл открыт, выполняем запись в файл результата
											if(file.is_open()){
												// Создаём текст для записи
												const string & text = alphabet.format("%ls\r\n", tmp.c_str());
												// Выполняем запись данных в файл
												file.write(text.data(), text.size());
											}
											// Если нужно выводить информацию статистики
											if(env.is("spell-verbose") && !info.empty()){
												// Выводим разделитель
												print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
												// Отображаем входящий текст
												print(alphabet.format("Text: %s\r\n", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Отображаем результат
												print(alphabet.format("Result: %ls\r\n", tmp.c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Отображаем загоовок списка исправлений
												print("The analytics:", env.get("log"), alphabet_t::log_t::null, false);
												// Переходим по всему блоку информации
												for(auto & item : info){
													// Переходим по всему списку вариантов
													for(auto & value : item){
														// Если варианты разные
														if(value.first.compare(value.second) != 0){
															// Если - это не существующий контекст
															if(value.second.compare(NOTFOUND) == 0){
																// Выводим результат в консоль
																print(alphabet.format("Info: [%ls] to [is not correct]", value.first.c_str()), env.get("log"), alphabet_t::log_t::null, false);
															// Выводим результат в консоль
															} else print(alphabet.format("Replace: [%ls] to [%ls]", value.first.c_str(), value.second.c_str()), env.get("log"), alphabet_t::log_t::null, false);
														// Выводим сообщение, что всё хорошо
														} else print(alphabet.format("Info: [%ls] to [correct]", value.first.c_str()), env.get("log"), alphabet_t::log_t::null, false);
													}
													// Выводим разделитель
													print("\r\n", env.get("log"), alphabet_t::log_t::null, false);
												}
												// Выводим сообщение о затраченном времени
												print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим разделитель
												print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
											}
										}
										// Если отладка включена
										if(!env.is("spell-verbose") && (debug > 0)){
											// Общий полученный размер данных
											size += text.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(fileSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									});
								}
								// Если файл открыт
								if(file.is_open()) file.close();
								// Если отладка включена
								if(!env.is("spell-verbose")){
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(100); break;
										case 2: pss.status(100); break;
									}
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						// Переводим работу в интерактивный режим
						} else if(env.is("interactive")) {
							// Текст для исправления
							wstring text = L"";
							// Буфер данных для ввода текста
							string buffer = "";
							// Информационные сообщения
							const string info1 = "To exit, enter", info2 = "Enter text for correction";
							// Выполняем очистку консоли
							printf("\033c");
							// Выводим сообщение
							printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
							// Выполняем чтение данных в буфер
							while(true){
								// Считываем данные в буфер
								getline(cin, buffer);
								// Если данные существуют
								if(!buffer.empty()){
									// Если это выход тогда выходим
									if(buffer.compare("end") == 0){
										// Выполняем очистку консоли
										printf("\033c");
										// Выводим прощание
										printf("\r\n\x1B[34m\x1B[1mGoodbye!\x1B[0m\r\n\r\n");
										// Выходим из приложения
										break;
									}
									// Получаем текст для исправления
									text = alphabet.convert(buffer);
									// Выполняем исправление опечаток
									spell.spell(text, options);
									// Выполняем очистку консоли
									printf("\033c");
									// Выводим сообщение
									printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
									// Выводим исходный текст
									printf("\r\n\x1B[1mOriginal text:\x1B[0m\r\n%s\r\n\r\n", buffer.c_str());
									// Выводим результат
									printf("\x1B[1mCorrected text:\x1B[0m\r\n%ls\r\n\r\n", text.c_str());
								}
							}
							// Выходим из приложения
							exit(0);
						// Сообщаем, что текст не указан
						} else print("text is empty or wrong params\r\n", env.get("log"));
					// Если нужно выполнять сплит по дефисам в тексте
					} else if(env.is("method", "hyphen")) {
						// Список последовательности
						vector <size_t> seq;
						// Список проспличенных слов
						vector <wstring> words;
						// Если текст передан
						if(((value = env.get("text")) != nullptr) && (string(value).compare("-yes-") != 0)){
							// Выполняем сплит слова
							spell.hyphen(alphabet.convert(value), options, seq, words);
							// Если список слов с ошибками получен
							if(!words.empty())
								// Выполняем восстановление контекста
								printf("%ls\r\n", tokenizer.restore(words).c_str());
							// Выводим текст как передан
							else printf("%s\r\n", value);
							// Выводим разделитель
							if(debug > 0) cout << endl;
						// Если адрес текстового файла или каталог передан
						} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value)) && ((debug == 0) || (env.get("log") != nullptr))) {
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Идентификатор документа
								size_t size = 0;
								// Статус и процентное соотношение
								u_short status = 0, rate = 100;
								// Запоминаем адрес файла
								const string & path = realpath(value, nullptr);
								// Расширение файлов текстового корпуса
								const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(path);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Split words in the text", "Split words in the text, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Замеряем время завершения работы анализатора
								auto end = chrono::system_clock::now();
								// Замеряем время работы анализатора
								auto start = chrono::system_clock::now();
								// Если это каталог
								if(fsys_t::isdir(path)){
									// Переходим по всему списку файлов в каталоге
									fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
										// Устанавливаем название файла
										if(debug > 0) pss.description(filename);
										// Выполняем считывание всех строк текста
										fsys_t::rfile2(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
											// Если текст получен
											if(!text.empty()){
												// Замеряем время работы анализатора
												start = chrono::system_clock::now();
												// Выполняем сплит слова
												spell.hyphen(alphabet.convert(text), options, seq, words);
												// Замеряем время завершения работы анализатора
												end = chrono::system_clock::now();
												// Выводим разделитель
												print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
												// Отображаем результат
												print(alphabet.format("Text: %s", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим получившийся текст
												print(alphabet.format("Split: %ls", (!words.empty() ? tokenizer.restore(words).c_str() : alphabet.convert(text).c_str())), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим сообщение о затраченном времени
												print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим разделитель
												print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
											}
											// Если отладка включена
											if(debug > 0){
												// Общий полученный размер данных
												size += text.size();
												// Подсчитываем статус выполнения
												status = u_short(size / double(dirSize) * 100.0);
												// Если процентное соотношение изменилось
												if(rate != status){
													// Запоминаем текущее процентное соотношение
													rate = status;
													// Отображаем ход процесса
													switch(debug){
														case 1: pss.update(status); break;
														case 2: pss.status(status); break;
													}
												}
											}
										});
									});
								// Если это не каталог а файл
								} else {
									// Выполняем считывание всех строк текста
									fsys_t::rfile(path, [&](const string & text, const uintmax_t fileSize) noexcept {
										// Если текст получен
										if(!text.empty()){
											// Замеряем время работы анализатора
											start = chrono::system_clock::now();
											// Выполняем сплит слова
											spell.hyphen(alphabet.convert(text), options, seq, words);
											// Замеряем время завершения работы анализатора
											end = chrono::system_clock::now();
											// Выводим разделитель
											print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
											// Отображаем результат
											print(alphabet.format("Text: %s", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
											// Выводим получившийся текст
											print(alphabet.format("Split: %ls", (!words.empty() ? tokenizer.restore(words).c_str() : alphabet.convert(text).c_str())), env.get("log"), alphabet_t::log_t::null, false);
											// Выводим сообщение о затраченном времени
											print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
											// Выводим разделитель
											print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
										}
										// Если отладка включена
										if(debug > 0){
											// Общий полученный размер данных
											size += text.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(fileSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									});
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						// Переводим работу в интерактивный режим
						} else if(env.is("interactive")) {
							// Текст для исправления
							wstring text = L"";
							// Буфер данных для ввода текста
							string buffer = "";
							// Список последовательности
							vector <size_t> seq;
							// Список проспличенных слов
							vector <wstring> words;
							// Информационные сообщения
							const string info1 = "To exit, enter", info2 = "Enter text for splited";
							// Выполняем очистку консоли
							printf("\033c");
							// Выводим сообщение
							printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
							// Выполняем чтение данных в буфер
							while(true){
								// Считываем данные в буфер
								getline(cin, buffer);
								// Если данные существуют
								if(!buffer.empty()){
									// Если это выход тогда выходим
									if(buffer.compare("end") == 0){
										// Выполняем очистку консоли
										printf("\033c");
										// Выводим прощание
										printf("\r\n\x1B[34m\x1B[1mGoodbye!\x1B[0m\r\n\r\n");
										// Выходим из приложения
										break;
									}
									// Получаем текст для исправления
									text = alphabet.convert(buffer);
									// Выполняем сплит слова
									spell.hyphen(text, options, seq, words);
									// Выполняем очистку консоли
									printf("\033c");
									// Выводим сообщение
									printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
									// Выводим исходный текст
									printf("\r\n\x1B[1mOriginal text:\x1B[0m\r\n%s\r\n\r\n", buffer.c_str());
									// Если список слов с ошибками получен
									if(!words.empty())
										// Выполняем восстановление контекста
										printf("\x1B[1mSplited text:\x1B[0m\r\n%ls\r\n\r\n", tokenizer.restore(words).c_str());
									// Выводим текст как передан
									else printf("\x1B[1mSplited text:\x1B[0m\r\n%ls\r\n\r\n", text.c_str());
								}
							}
							// Выходим из приложения
							exit(0);
						// Сообщаем, что текст не указан
						} else print("text is empty or wrong params\r\n", env.get("log"));
					// Если нужно выполнить анализ текста
					} else if(env.is("method", "analyze")) {
						// Информационные собранные данные аналитики
						vector <unordered_map <wstring, set <wstring>>> info;
						// Если текст передан
						if(((value = env.get("text")) != nullptr) && (string(value).compare("-yes-") != 0)){
							// Выполняем сборку данных
							spell.analyze(alphabet.convert(value), options, info);
							// Если анализ получен
							if(!info.empty()){
								// Список собранных слов
								wstring words = L"";
								// Переходим по всему списку
								for(auto & item : info){
									// Переходим по всему списку вариантов
									for(auto & value : item){
										// Очищаем список собранных слов
										words.clear();
										// Переходим по остальным вариантам
										for(auto & item : value.second){
											// Если слово не совпадает с основным словом
											if(value.first.compare(item) != 0){
												// Если слово не пустое, добавляем разделитель
												if(!words.empty()) words.append(L", ");
												// Если - это неверный контекст
												if(item.compare(NOTFOUND) == 0)
													// Сообщаем, что контекст неверный
													words.append(L"is not correct");
												// Добавляем слово в список
												else words.append(item);
											}
										}
										// Выводим скобку
										if(!words.empty()) printf("%ls [%ls]\r\n", value.first.c_str(), words.c_str());
									}
									// Выводим разделитель
									if(debug > 0) cout << endl;
								}
							}
						// Если адрес текстового файла или каталог передан
						} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value)) && ((debug == 0) || (env.get("log") != nullptr))) {
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Идентификатор документа
								size_t size = 0;
								// Статус и процентное соотношение
								u_short status = 0, rate = 100;
								// Запоминаем адрес файла
								const string & path = realpath(value, nullptr);
								// Расширение файлов текстового корпуса
								const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(path);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Analyze in the text", "Analyze in the text, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Замеряем время завершения работы анализатора
								auto end = chrono::system_clock::now();
								// Замеряем время работы анализатора
								auto start = chrono::system_clock::now();
								// Если это каталог
								if(fsys_t::isdir(path)){
									// Переходим по всему списку файлов в каталоге
									fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
										// Устанавливаем название файла
										if(debug > 0) pss.description(filename);
										// Выполняем считывание всех строк текста
										fsys_t::rfile2(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
											// Если текст получен
											if(!text.empty()){
												// Замеряем время работы анализатора
												start = chrono::system_clock::now();
												// Выполняем сборку данных
												spell.analyze(alphabet.convert(text), options, info);
												// Замеряем время завершения работы анализатора
												end = chrono::system_clock::now();
												// Если анализ получен
												if(!info.empty()){
													// Список собранных слов
													wstring words = L"";
													// Выводим разделитель
													print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
													// Отображаем результат
													print(alphabet.format("Text: %s\r\n", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
													// Переходим по всему списку
													for(auto & item : info){
														// Переходим по всему списку вариантов
														for(auto & value : item){
															// Очищаем список собранных слов
															words.clear();
															// Переходим по остальным вариантам
															for(auto & item : value.second){
																// Если слово не совпадает с основным словом
																if(value.first.compare(item) != 0){
																	// Если слово не пустое, добавляем разделитель
																	if(!words.empty()) words.append(L", ");
																	// Если - это неверный контекст
																	if(item.compare(NOTFOUND) == 0)
																		// Сообщаем, что контекст неверный
																		words.append(L"is not correct");
																	// Добавляем слово в список
																	else words.append(item);
																}
															}
															// Выводим скобку
															if(!words.empty()) print(alphabet.format("Variants: [%ls] to [%ls]", value.first.c_str(), words.c_str()), env.get("log"), alphabet_t::log_t::null, false);
														}
													}
													// Выводим разделитель
													print("\r\n", env.get("log"), alphabet_t::log_t::null, false);
													// Выводим сообщение о затраченном времени
													print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
													// Выводим разделитель
													print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
												}
											}
											// Если отладка включена
											if(debug > 0){
												// Общий полученный размер данных
												size += text.size();
												// Подсчитываем статус выполнения
												status = u_short(size / double(dirSize) * 100.0);
												// Если процентное соотношение изменилось
												if(rate != status){
													// Запоминаем текущее процентное соотношение
													rate = status;
													// Отображаем ход процесса
													switch(debug){
														case 1: pss.update(status); break;
														case 2: pss.status(status); break;
													}
												}
											}
										});
									});
								// Если это не каталог а файл
								} else {
									// Выполняем считывание всех строк текста
									fsys_t::rfile(path, [&](const string & text, const uintmax_t fileSize) noexcept {
										// Если текст получен
										if(!text.empty()){
											// Замеряем время работы анализатора
											start = chrono::system_clock::now();
											// Выполняем сборку данных
											spell.analyze(alphabet.convert(text), options, info);
											// Замеряем время завершения работы анализатора
											end = chrono::system_clock::now();
											// Если анализ получен
											if(!info.empty()){
												// Список собранных слов
												wstring words = L"";
												// Выводим разделитель
												print("============= BEGIN =============", env.get("log"), alphabet_t::log_t::null, false);
												// Отображаем результат
												print(alphabet.format("Text: %s\r\n", text.c_str()), env.get("log"), alphabet_t::log_t::null, false);
												// Переходим по всему списку
												for(auto & item : info){
													// Переходим по всему списку вариантов
													for(auto & value : item){
														// Очищаем список собранных слов
														words.clear();
														// Переходим по остальным вариантам
														for(auto & item : value.second){
															// Если слово не совпадает с основным словом
															if(value.first.compare(item) != 0){
																// Если слово не пустое, добавляем разделитель
																if(!words.empty()) words.append(L", ");
																// Добавляем слово в список
																words.append(item);
															}
														}
														// Выводим скобку
														if(!words.empty()) print(alphabet.format("Variants: [%ls] to [%ls]", value.first.c_str(), words.c_str()), env.get("log"), alphabet_t::log_t::null, false);
													}
												}
												// Выводим разделитель
												print("\r\n", env.get("log"), alphabet_t::log_t::null, false);
												// Выводим сообщение о затраченном времени
												print(alphabet.format("Time shifting: %lldms", chrono::duration_cast <chrono::milliseconds> (end - start).count()), env.get("log"), alphabet_t::log_t::null, false);
												// Выводим разделитель
												print("============= END =============\r\n", env.get("log"), alphabet_t::log_t::null, false);
											}
										}
										// Если отладка включена
										if(debug > 0){
											// Общий полученный размер данных
											size += text.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(fileSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									});
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						// Переводим работу в интерактивный режим
						} else if(env.is("interactive")) {
							// Текст для исправления
							wstring text = L"";
							// Буфер данных для ввода текста
							string buffer = "";
							// Информационные собранные данные аналитики
							vector <unordered_map <wstring, set <wstring>>> info;
							// Информационные сообщения
							const string info1 = "To exit, enter", info2 = "Enter text for analyze";
							// Выполняем очистку консоли
							printf("\033c");
							// Выводим сообщение
							printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
							// Выполняем чтение данных в буфер
							while(true){
								// Считываем данные в буфер
								getline(cin, buffer);
								// Если данные существуют
								if(!buffer.empty()){
									// Если это выход тогда выходим
									if(buffer.compare("end") == 0){
										// Выполняем очистку консоли
										printf("\033c");
										// Выводим прощание
										printf("\r\n\x1B[34m\x1B[1mGoodbye!\x1B[0m\r\n\r\n");
										// Выходим из приложения
										break;
									}
									// Получаем текст для исправления
									text = alphabet.convert(buffer);
									// Выполняем сборку данных
									spell.analyze(text, options, info);
									// Выполняем очистку консоли
									printf("\033c");
									// Выводим сообщение
									printf("\r\n\x1B[1m%s.\x1B[0m\r\n\x1B[1m%s:\x1B[0m end\r\n\r\n", info2.c_str(), info1.c_str());
									// Выводим исходный текст
									printf("\r\n\x1B[1mOriginal text:\x1B[0m\r\n%s\r\n\r\n", buffer.c_str());
									// Выводим результат
									printf("\x1B[1m%s:\x1B[0m\r\n", "Analyze text");
									// Если анализ получен
									if(!info.empty()){
										// Список собранных слов
										wstring words = L"";
										// Переходим по всему списку
										for(auto & item : info){
											// Переходим по всему списку вариантов
											for(auto & value : item){
												// Очищаем список собранных слов
												words.clear();
												// Переходим по остальным вариантам
												for(auto & item : value.second){
													// Если слово не совпадает с основным словом
													if(value.first.compare(item) != 0){
														// Если слово не пустое, добавляем разделитель
														if(!words.empty()) words.append(L", ");
														// Если - это неверный контекст
														if(item.compare(NOTFOUND) == 0)
															// Сообщаем, что контекст неверный
															words.append(L"is not correct");
														// Добавляем слово в список
														else words.append(item);
													}
												}
												// Выводим скобку
												if(!words.empty()) printf("%ls [%ls]\r\n", value.first.c_str(), words.c_str());
											}
											// Выводим разделитель
											if(!words.empty() && (debug > 0)) cout << endl;
										}
									}
								}
							}
							// Выходим из приложения
							exit(0);
						// Сообщаем, что текст не указан
						} else print("text is empty or wrong params\r\n", env.get("log"));
					// Сообщаем, что метод неизвестный
					} else print("method is unknown\r\n", env.get("log"));
				// Сообщаем, что метод неизвестный
				} else print("method is unknown\r\n", env.get("log"));
				// Если режим отладки включён
				if(debug > 0){
					// Получаем диапазон времени
					auto dimension = pss.dimension(chrono::duration_cast <chrono::seconds> (chrono::system_clock::now() - timeShifting).count());
					// Выводим результат
					print(alphabet.format("work time shifting: %lld %s\r\n", dimension.first, dimension.second.c_str()), env.get("log"), alphabet_t::log_t::info);
				// Иначе просто нормально выходим
				} else exit(0);
			// Если это метод обучения спеллчекера
			} else if(env.is("method", "train")) {
				// Создаём объект тулкита языковой модели
				toolkit_t toolkit(&alphabet, &tokenizer, order);
				// Если нужно провести обучение, полностью с нуля
				if(((value = env.get("corpus")) != nullptr) && (fsys_t::isdir(value) || fsys_t::isfile(value))){
					// Устанавливаем адрес файла для логирования
					toolkit.setLogfile(env.get("log"));
					// Устанавливаем режим отладки
					if(debug == 2) toolkit.setOption(toolkit_t::options_t::debug);
					// Разрешаем использовать токен неизвестного слова
					if(env.is("allow-unk")) toolkit.setOption(toolkit_t::options_t::allowUnk);
					// Разрешаем сбрасывать частоту токена неизвестного слова
					if(env.is("reset-unk")) toolkit.setOption(toolkit_t::options_t::resetUnk);
					// Разрешаем использовать абсолютно все полученные n-граммы
					if(env.is("all-grams")) toolkit.setOption(toolkit_t::options_t::allGrams);
					// Устанавливаем запрет на все слова кроме тех, что находятся в белом списке
					if(env.is("only-good")) toolkit.setOption(toolkit_t::options_t::onlyGood);
					// Разрешаем переводить слова в нижний регистр
					if(env.is("lower-case")) toolkit.setOption(toolkit_t::options_t::lowerCase);
					// Разрешаем детектировать слова состоящее из смешанных словарей
					if(env.is("mixed-dicts")) toolkit.setOption(toolkit_t::options_t::mixdicts);
					// Разрешаем выполнять загрузку содержимого arpa, в том виде, в каком она есть. Без перетокенизации содержимого.
					if(env.is("confidence")) toolkit.setOption(toolkit_t::options_t::confidence);
					// Разрешаем выполнять интерполяцию при расчёте arpa
					if(env.is("interpolate")) toolkit.setOption(toolkit_t::options_t::interpolate);
					// Флаг учитывающий при сборке N-грамм, только те токены, которые соответствуют словам
					if(env.is("only-token-words")) toolkit.setOption(toolkit_t::options_t::tokenWords);
					// Если нужно установить все токены для идентифицирования как <unk>
					if(env.is("tokens-all-unknown")) toolkit.setAllTokenUnknown();
					// Если нужно установить все токены как не идентифицируемые
					if(env.is("tokens-all-disable")) toolkit.setAllTokenDisable();
					// Если нужно установить список токенов которые нужно идентифицировать как <unk>
					if((value = env.get("tokens-unknown")) != nullptr) toolkit.setTokenUnknown(value);
					// Если нужно установить список не идентифицируемых токенов
					if((value = env.get("tokens-disable")) != nullptr) toolkit.setTokenDisable(value);
					// Если адрес файла идентификаторов аббревиатур передан
					if((value = env.get("r-abbrs-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Список токенов которые нужно идентифицировать как <unk>
							set <size_t> tokens;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read abbreviations file", "Read abbreviations file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для списка аббревиатур
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									tokens.emplace(stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если список токенов получен, устанавливаем его
							if(!tokens.empty()) tokenizer.setAbbrs(tokens);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла токенов которые нужно идентифицировать как <unk>
					if((value = env.get("r-tokens-unknown-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Список токенов которые нужно идентифицировать как <unk>
							set <token_t> tokens;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read tokens unknown file", "Read tokens unknown file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для списка токенов которые нужно идентифицировать как <unk>
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									tokens.emplace((token_t) stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если список токенов получен, устанавливаем его
							if(!tokens.empty()) toolkit.setTokensUnknown(tokens);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла не идентифицируемых токенов получен
					if((value = env.get("r-tokens-disable-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Список не идентифицируемых токенов
							set <token_t> tokens;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read tokens disable file", "Read tokens disable file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для списка не идентифицируемых токенов
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									tokens.emplace((token_t) stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если список токенов получен, устанавливаем его
							if(!tokens.empty()) toolkit.setTokensDisable(tokens);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла чёрного списка получен
					if((value = env.get("badwords")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Чёрный список слов
							vector <string> badwords;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read badwords file", "Read badwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для чёрного списка
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									badwords.push_back(word);
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если чёрный список получен, устанавливаем его
							if(!badwords.empty()) toolkit.setBadwords(badwords);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла белого списка получен
					if((value = env.get("goodwords")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Белый список слов
							vector <string> goodwords;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read goodwords file", "Read goodwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для белого списка
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если слово получено
								if(!word.empty()){
									// Добавляем в список, полученное слово
									goodwords.push_back(word);
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если белый список получен, устанавливаем его
							if(!goodwords.empty()) toolkit.setGoodwords(goodwords);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла белого списка идентификаторв получен
					if((value = env.get("r-badwords-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Чёрный список слов
							set <size_t> badwords;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read badwords file", "Read badwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для чёрного списка
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									badwords.emplace(stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если чёрный список получен, устанавливаем его
							if(!badwords.empty()) toolkit.setBadwords(badwords);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла белого списка идентификаторв получен
					if((value = env.get("r-goodwords-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Белый список слов
							set <size_t> goodwords;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read goodwords file", "Read goodwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для белого списка
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если слово получено
								if(!word.empty()){
									// Добавляем в список, полученное слово
									goodwords.emplace(stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если белый список получен, устанавливаем его
							if(!goodwords.empty()) toolkit.setGoodwords(goodwords);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если неизвестное слово получено
					if((value = env.get("unknown-word")) != nullptr) toolkit.setUnknown(value);
					// Если пользовательские токены получены
					if(((value = env.get("utokens")) != nullptr) && (string(value).compare("-yes-") != 0)){
						// Список пользовательских токенов
						vector <wstring> tokens;
						// Выполняем извлечение пользовательских токенов
						alphabet.split(value, "|", tokens);
						// Если список токенов получен
						if(!tokens.empty()){
							// Если адрес скрипта получен
							if((value = env.get("utoken-script")) != nullptr){
								// Устанавливаем адрес скрипта
								toolkit.setUserTokenScript(value);
								// Переходим по всему списку токенов
								for(auto & item : tokens) toolkit.setUserToken(alphabet.convert(item));
							}
						}
					}
					// Если адрес скрипта получен
					if((value = env.get("word-script")) != nullptr) toolkit.setWordScript(value);
					// Выполняем инициализацию алгоритма сглаживания
					if(env.is("smoothing")){
						// Дополнительный коэффициент алгоритма сглаживания
						double mod = 0.0;
						// Если алгоритм сглаживания ConstDiscount или AddSmooth, запрашиваем дополнительные параметры
						if(env.is("smoothing", "cdiscount") || env.is("smoothing", "addsmooth")){
							// Считываем флаг дополнительной модификации
							value = (env.is("smoothing", "cdiscount") ? env.get("discount") : env.get("delta"));
							// Если значение получено
							if(value != nullptr) mod = stod(value);
						}
						// Если это WittenBell
						if(env.is("smoothing", "wittenbell")) toolkit.init(toolkit_t::algorithm_t::wittenBell, false, false, 0.0);
						// Если это AddSmooth
						else if(env.is("smoothing", "addsmooth")) toolkit.init(toolkit_t::algorithm_t::addSmooth, false, false, mod);
						// Если это GoodTuring
						else if(env.is("smoothing", "goodturing")) toolkit.init(toolkit_t::algorithm_t::goodTuring, false, false, 0.0);
						// Если это ConstDiscount
						else if(env.is("smoothing", "cdiscount")) toolkit.init(toolkit_t::algorithm_t::constDiscount, false, false, mod);
						// Если это NaturalDiscount
						else if(env.is("smoothing", "ndiscount")) toolkit.init(toolkit_t::algorithm_t::naturalDiscount, false, false, 0.0);
						// Если это KneserNey
						else if(env.is("smoothing", "kneserney")) toolkit.init(toolkit_t::algorithm_t::kneserNey, env.is("kneserney-modified"), env.is("kneserney-prepares"), 0.0);
						// Если это ModKneserNey
						else if(env.is("smoothing", "mkneserney")) toolkit.init(toolkit_t::algorithm_t::modKneserNey, env.is("kneserney-modified"), env.is("kneserney-prepares"), 0.0);
						// Сообщаем, что сглаживание выбрано не верно
						else print("smoothing is bad\r\n", env.get("log"));
					// Сообщаем, что сглаживание выбрано не верно
					} else print("smoothing is bad\r\n", env.get("log"));
					// Если путь получен
					if(((value = env.get("corpus")) != nullptr) && fsys_t::isdir(value)){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Запоминаем путь к файлам
							const string & path = realpath(value, nullptr);
							// Расширение файлов текстового корпуса
							const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
							// Если количество ядер передано
							if(((value = env.get("threads")) != nullptr) && alphabet.isNumber(value)){
								// Объявляем объект коллектора
								collector_t collector(&toolkit, &alphabet, &tokenizer, env.get("log"));
								// Устанавливаем режим отладки
								collector.setDebug(debug);
								// Устанавливаем размер n-граммы
								collector.setOrder(order);
								// Устанавливаем количество потоков
								collector.setThreads(stoi(value));
								// Устанавливаем флаг автоочистки
								collector.setIntermed(env.is("train-intermed"));
								// Устанавливаем флаг сегментации
								collector.setSegment(
									env.is("train-segments"),
									((value = env.get("train-segment-size")) != nullptr ? value : "auto")
								);
								// Устанавливаем путь назначения
								if((value = env.get("train-intermed-dest")) != nullptr) collector.setDest(value);
								// Выполняем чтение данных каталога
								collector.readDir(path, ext);
							// Иначе выполняем сборку обычным способом
							} else {
								// Идентификатор документа
								size_t idd = 0, size = 0;
								// Статус и процентное соотношение
								u_short status = 0, rate = 100;
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Устанавливаем заголовки прогресс-бара
									pss.title("Load text corpus", "Load text corpus, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Переходим по всему списку файлов в каталоге
								fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
									// Устанавливаем название файла
									if(debug > 0) pss.description(filename);
									// Выполняем считывание всех строк текста
									fsys_t::rfile2(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
										// Если текст получен
										if(!text.empty()) toolkit.addText(text, idd);
										// Если отладка включена
										if(debug > 0){
											// Общий полученный размер данных
											size += text.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(dirSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									});
									// Увеличиваем идентификатор документа
									idd++;
								});
							}
							// Если файл arpa для записи указан
							if(env.is("w-arpa") || env.is("w-bin")){
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем заголовки прогресс-бара
									pss.title("Train arpa", "Train arpa, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем обучение
								toolkit.train([debug, &pss](const u_short status) noexcept {
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					// Если файл корпуса получен
					} else if(((value = env.get("corpus")) != nullptr) && fsys_t::isfile(value)){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Запоминаем адрес файла
							const string & filename = realpath(value, nullptr);
							// Если количество ядер передано
							if(((value = env.get("threads")) != nullptr) && alphabet.isNumber(value)){
								// Объявляем объект коллектора
								collector_t collector(&toolkit, &alphabet, &tokenizer, env.get("log"));
								// Устанавливаем режим отладки
								collector.setDebug(debug);
								// Устанавливаем размер n-граммы
								collector.setOrder(order);
								// Устанавливаем количество потоков
								collector.setThreads(stoi(value));
								// Устанавливаем флаг автоочистки
								collector.setIntermed(env.is("train-intermed"));
								// Устанавливаем флаг сегментации
								collector.setSegment(
									env.is("train-segments"),
									((value = env.get("train-segment-size")) != nullptr ? value : "auto")
								);
								// Устанавливаем путь назначения
								if((value = env.get("train-intermed-dest")) != nullptr) collector.setDest(value);
								// Выполняем чтение данных файла
								collector.readFile(filename);
							// Иначе выполняем сборку обычным способом
							} else {
								// Идентификатор документа
								size_t size = 0;
								// Статус и процентное соотношение
								u_short status = 0, rate = 100;
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Устанавливаем название файла
									pss.description(filename);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Load text corpus", "Load text corpus, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем считывание всех строк текста
								fsys_t::rfile(filename, [&rate, &status, &size, debug, &pss, &toolkit](const string & text, const uintmax_t fileSize) noexcept {
									// Если текст получен
									if(!text.empty()) toolkit.addText(text, 0);
									// Если отладка включена
									if(debug > 0){
										// Общий полученный размер данных
										size += text.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								});
							}
							// Если файл arpa для записи указан
							if(env.is("w-arpa") || env.is("w-bin")){
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем заголовки прогресс-бара
									pss.title("Train arpa", "Train arpa, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем обучение
								toolkit.train([debug, &pss](const u_short status) noexcept {
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					// Если путь не указан
					} else print("path or file with corpus texts is not specified\r\n", env.get("log"));
					// Если нужно выполнить прунинг словаря
					if(env.is("vprune") && (env.is("vprune-wltf") || env.is("vprune-oc") || env.is("vprune-dc"))) {
						// Получаем порог встречаемости слова
						const size_t oc = (((value = env.get("vprune-oc")) != nullptr) && alphabet.isNumber(value) ? stoull(value) : 0);
						// Получаем порог количества документов, где встретилось слово
						const size_t dc = (((value = env.get("vprune-dc")) != nullptr) && alphabet.isNumber(value) ? stoull(value) : 0);
						// Получаем порог веса слова
						const double wltf = (((value = env.get("vprune-wltf")) != nullptr) && alphabet.isDecimal(value) ? stod(value) : 0.0);
						// Проверяем на правильность входных параметров
						if((wltf != 0.0) || (oc > 0) || (dc > 0)){
							// Если отладка включена, выводим индикатор прунинга
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Prune vocab", "Prune vocab, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем прунинг словаря
							toolkit.pruneVocab(wltf, oc, dc, [debug, &pss](const u_short status) noexcept {
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						// Сообщаем, что порог передан неверный
						} else print("threshold is broken\r\n", env.get("log"));
					}
					// Если нужно выполнить прунинг arpa
					if(env.is("aprune")){
						// Максимальный размер n-граммы
						u_short size = 0;
						// Коэффициент прунинга
						double prune = 0.0;
						// Если параметр максимального размера n-граммы для прунинга получен
						if((value = env.get("aprune-max-gram")) != nullptr) size = stoi(value);
						// Если параметр прунинга получен
						if((value = env.get("aprune-threshold")) != nullptr) prune = stod(value);
						// Если параметры получены
						if((size > 0) && (prune != 0.0)){
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Prune arpa", "Prune arpa, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем прунинг
							toolkit.prune(prune, size, [debug, &pss](const u_short status) noexcept {
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						}
					}
					// Если требуется загрузить файл списка слов
					if(((value = env.get("r-words")) != nullptr) && fsys_t::isfile(value)){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем название файла
								pss.description(filename);
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read words file", "Read words file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем загрузку файла словаря списка слов
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если слово получено
								if(!word.empty()){
									// Добавляем слово в словарь
									toolkit.addWord(alphabet.convert(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					// Если требуется загрузить каталог списка слов
					} else if(((value = env.get("r-words")) != nullptr) && fsys_t::isdir(value)) {
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0, index = 0;
							// Запоминаем каталог для загрузки
							const string & path = realpath(value, nullptr);
							// Расширение файлов текстового корпуса
							const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read words file", "Read words file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Переходим по всему списку словарей в каталоге
							fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
								// Если отладка включена, выводим название файла
								if(debug > 0) pss.description(filename);
								// Выполняем загрузку файла словаря списка слов
								fsys_t::rfile2(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
									// Если слово получено
									if(!word.empty()){
										// Добавляем слово в словарь
										toolkit.addWord(alphabet.convert(word), 0, index);
										// Если отладка включена, выводим индикатор загрузки
										if(debug > 0){
											// Общий полученный размер данных
											size += word.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(dirSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									}
								});
								// Увеличиваем количество обработанных документов
								index++;
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если файл для сохранения конфигурационных данных передан
					if((value = env.get("w-json")) != nullptr){
						// Получаем конфигурационные данные в формате JSON
						const json & config = env.getJson();
						// Если конфигурационные данные получены
						if(!config.empty()){
							// Открываем файл на запись
							ofstream file(value, ios::binary);
							// Если файл открыт
							if(file.is_open()){
								// Формируем текст результата
								const string & text = config.dump(4);
								// Если текст получен, записываем данные в файл
								if(!text.empty()) file.write(text.data(), text.size());
								// Закрываем файл
								file.close();
							}
						}
					}
					// Если файл для сохранения слов передан
					if((value = env.get("w-words")) != nullptr){
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(value);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Write words", "Write words, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем извлечение слов в файл
						toolkit.writeWords(value, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
					// Если файл для сохранения vocab передан
					if((value = env.get("w-vocab")) != nullptr){
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(value);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Write vocab", "Write vocab, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем извлечение словаря в файл
						toolkit.writeVocab(value, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
					// Если файл для извлечения карты последовательности передан
					if((value = env.get("w-map")) != nullptr){
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(value);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Write map", "Write map, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем сохранение каты последовательности
						toolkit.writeMap(value, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
					// Если файл для сохранения arpa передан
					if((value = env.get("w-arpa")) != nullptr){
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(value);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Write arpa", "Write arpa, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем сохранение arpa файла
						toolkit.writeArpa(value, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
					// Если файл для сохранения n-грамм передан
					if((value = env.get("w-ngram")) != nullptr){
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(value);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Write ngrams", "Write ngrams, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем сохранение файлов n-грамм
						toolkit.writeNgrams(value, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
					// Если адрес файла списка суффиксов цифровых аббревиатур получен
					if((value = env.get("w-abbr")) != nullptr){
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(value);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Write abbrs", "Write abbrs, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем запись данных аббревиатур в файл
						tokenizer.writeSuffix(value, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
					// Если адрес файла идентификаторов аббревиатур передан
					if((value = env.get("w-abbrs-idw")) != nullptr){
						// Получаем список токенов
						const auto & tokens = tokenizer.getAbbrs();
						// Если список токенов получен
						if(!tokens.empty()){
							// Открываем файл на запись
							ofstream file(value, ios::binary);
							// Если файл открыт, выполняем запись в файл результата
							if(file.is_open()){
								// Параметры индикаторы процесса
								size_t index = 0, status = 0, rate = 0;
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(value);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Write abbreviations", "Write abbreviations, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Переходим по всему списку токенов
								for(auto & token : tokens){
									// Создаём текст для записи
									const string & text = alphabet.format("%zu\r\n", token);
									// Выполняем запись данных в файл
									file.write(text.data(), text.size());
									// Если отладка включена
									if(debug > 0){
										// Общий полученный размер данных
										index++;
										// Подсчитываем статус выполнения
										status = u_short(index / double(tokens.size()) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
								// Закрываем файл
								file.close();
							}
						}
					}
					// Если адрес файла токенов которые нужно идентифицировать как <unk>
					if((value = env.get("w-tokens-unknown-idw")) != nullptr){
						// Получаем список токенов
						const auto & tokens = toolkit.getTokensUnknown();
						// Если список токенов получен
						if(!tokens.empty()){
							// Открываем файл на запись
							ofstream file(value, ios::binary);
							// Если файл открыт, выполняем запись в файл результата
							if(file.is_open()){
								// Параметры индикаторы процесса
								size_t index = 0, status = 0, rate = 0;
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(value);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Write tokens unknown", "Write tokens unknown, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Переходим по всему списку токенов
								for(auto & token : tokens){
									// Создаём текст для записи
									const string & text = alphabet.format("%zu\r\n", (size_t) token);
									// Выполняем запись данных в файл
									file.write(text.data(), text.size());
									// Если отладка включена
									if(debug > 0){
										// Общий полученный размер данных
										index++;
										// Подсчитываем статус выполнения
										status = u_short(index / double(tokens.size()) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
								// Закрываем файл
								file.close();
							}
						}
					}
					// Если адрес файла не идентифицируемых токенов получен
					if((value = env.get("w-tokens-disable-idw")) != nullptr){
						// Получаем список токенов
						const auto & tokens = toolkit.getTokensDisable();
						// Если список токенов получен
						if(!tokens.empty()){
							// Открываем файл на запись
							ofstream file(value, ios::binary);
							// Если файл открыт, выполняем запись в файл результата
							if(file.is_open()){
								// Параметры индикаторы процесса
								size_t index = 0, status = 0, rate = 0;
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(value);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Write tokens disable", "Write tokens disable, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Переходим по всему списку токенов
								for(auto & token : tokens){
									// Создаём текст для записи
									const string & text = alphabet.format("%zu\r\n", (size_t) token);
									// Выполняем запись данных в файл
									file.write(text.data(), text.size());
									// Если отладка включена
									if(debug > 0){
										// Общий полученный размер данных
										index++;
										// Подсчитываем статус выполнения
										status = u_short(index / double(tokens.size()) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
								// Закрываем файл
								file.close();
							}
						}
					}
					// Если адрес файла чёрного списка получен
					if((value = env.get("w-badwords-idw")) != nullptr){
						// Получаем список токенов
						const auto & tokens = toolkit.getBadwords();
						// Если список токенов получен
						if(!tokens.empty()){
							// Открываем файл на запись
							ofstream file(value, ios::binary);
							// Если файл открыт, выполняем запись в файл результата
							if(file.is_open()){
								// Параметры индикаторы процесса
								size_t index = 0, status = 0, rate = 0;
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(value);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Write badwords", "Write badwords, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Переходим по всему списку токенов
								for(auto & token : tokens){
									// Создаём текст для записи
									const string & text = alphabet.format("%zu\r\n", token);
									// Выполняем запись данных в файл
									file.write(text.data(), text.size());
									// Если отладка включена
									if(debug > 0){
										// Общий полученный размер данных
										index++;
										// Подсчитываем статус выполнения
										status = u_short(index / double(tokens.size()) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
								// Закрываем файл
								file.close();
							}
						}
					}
					// Если адрес файла белого списка идентификаторв получен
					if((value = env.get("w-goodwords-idw")) != nullptr){
						// Получаем список токенов
						const auto & tokens = toolkit.getGoodwords();
						// Если список токенов получен
						if(!tokens.empty()){
							// Открываем файл на запись
							ofstream file(value, ios::binary);
							// Если файл открыт, выполняем запись в файл результата
							if(file.is_open()){
								// Параметры индикаторы процесса
								size_t index = 0, status = 0, rate = 0;
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(value);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Write goodwords", "Write goodwords, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Переходим по всему списку токенов
								for(auto & token : tokens){
									// Создаём текст для записи
									const string & text = alphabet.format("%zu\r\n", token);
									// Выполняем запись данных в файл
									file.write(text.data(), text.size());
									// Если отладка включена
									if(debug > 0){
										// Общий полученный размер данных
										index++;
										// Подсчитываем статус выполнения
										status = u_short(index / double(tokens.size()) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
								// Закрываем файл
								file.close();
							}
						}
					}
				}
				// Если файл для бинарного словаря, передан
				if((value = env.get("w-bin")) != nullptr){
					// Запоминаем адрес файла для сохранения
					binDictFile = value;
					// Создаём обхъект языковой модели
					unique_ptr <alm_t> alm(env.is("alm2") ? new alm2_t(&alphabet, &tokenizer) : new alm1_t(&alphabet, &tokenizer));
					// Устанавливаем адрес файла для логирования
					alm->setLogfile(env.get("log"));
					// Если количество ядер передано
					if(((value = env.get("threads")) != nullptr) && alphabet.isNumber(value)){
						// Устанавливаем количество потоков
						alm->setThreads(stoi(value));
					// Иначе устанавливаем 1 поток
					} else alm->setThreads(1);
					// Устанавливаем режим отладки
					if(debug == 2) alm->setOption(alm_t::options_t::debug);
					// Разрешаем детектировать слова состоящее из смешанных словарей
					if(env.is("mixed-dicts")) alm->setOption(alm_t::options_t::mixdicts);
					// Разрешаем выполнять загрузку содержимого arpa, в том виде, в каком она есть. Без перетокенизации содержимого.
					if(env.is("confidence")) alm->setOption(alm_t::options_t::confidence);
					// Если нужно установить все токены для идентифицирования как <unk>
					if(env.is("tokens-all-unknown")) alm->setAllTokenUnknown();
					// Если нужно установить все токены как не идентифицируемые
					if(env.is("tokens-all-disable")) alm->setAllTokenDisable();
					// Если неизвестное слово получено
					if((value = env.get("unknown-word")) != nullptr) alm->setUnknown(value);
					// Если адрес скрипта получен
					if((value = env.get("word-script")) != nullptr) alm->setWordScript(value);
					// Если нужно установить список токенов которые нужно идентифицировать как <unk>
					if((value = env.get("tokens-unknown")) != nullptr) alm->setTokenUnknown(value);
					// Если нужно установить список не идентифицируемых токенов
					if((value = env.get("tokens-disable")) != nullptr) alm->setTokenDisable(value);
					// Если требуется загрузить файл словаря abbr
					if((value = env.get("r-abbr")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Запоминаем адрес файла
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем название файла
								pss.description(filename);
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read abbr", "Read abbr, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем загрузку файла суффиксов цифровых аббревиатур
							tokenizer.readSuffix(filename, [debug, &pss](const string & filename, const u_short status) noexcept {
								// Если отладка включена, выводим имя файла
								if(debug > 0) pss.description(filename);
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла идентификаторов аббревиатур передан
					if((value = env.get("r-abbrs-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Список токенов которые нужно идентифицировать как <unk>
							set <size_t> tokens;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read abbreviation file", "Read abbreviation file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для списка аббревиатур
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									tokens.emplace(stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если список токенов получен, устанавливаем его
							if(!tokens.empty()) tokenizer.setAbbrs(tokens);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла токенов которые нужно идентифицировать как <unk>
					if((value = env.get("r-tokens-unknown-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Список токенов которые нужно идентифицировать как <unk>
							set <token_t> tokens;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read tokens unknown file", "Read tokens unknown file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для списка токенов которые нужно идентифицировать как <unk>
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									tokens.emplace((token_t) stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если список токенов получен, устанавливаем его
							if(!tokens.empty()) alm->setTokensUnknown(tokens);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла не идентифицируемых токенов получен
					if((value = env.get("r-tokens-disable-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Список не идентифицируемых токенов
							set <token_t> tokens;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read tokens disable file", "Read tokens disable file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для списка не идентифицируемых токенов
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									tokens.emplace((token_t) stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если список токенов получен, устанавливаем его
							if(!tokens.empty()) alm->setTokensDisable(tokens);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла чёрного списка получен
					if((value = env.get("badwords")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Чёрный список слов
							vector <string> badwords;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read badwords file", "Read badwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для чёрного списка
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									badwords.push_back(word);
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если чёрный список получен, устанавливаем его
							if(!badwords.empty()) alm->setBadwords(badwords);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла белого списка получен
					if((value = env.get("goodwords")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Белый список слов
							vector <string> goodwords;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read goodwords file", "Read goodwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для белого списка
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если слово получено
								if(!word.empty()){
									// Добавляем в список, полученное слово
									goodwords.push_back(word);
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если белый список получен, устанавливаем его
							if(!goodwords.empty()) alm->setGoodwords(goodwords);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла чёрного списка получен
					if((value = env.get("r-badwords-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Чёрный список слов
							set <size_t> badwords;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read badwords file", "Read badwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для чёрного списка
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Добавляем в список, полученное слово
									badwords.emplace(stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если чёрный список получен, устанавливаем его
							if(!badwords.empty()) alm->setBadwords(badwords);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если адрес файла белого списка идентификаторв получен
					if((value = env.get("r-goodwords-idw")) != nullptr){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Белый список слов
							set <size_t> goodwords;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read goodwords file", "Read goodwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех слов для белого списка
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если слово получено
								if(!word.empty()){
									// Добавляем в список, полученное слово
									goodwords.emplace(stoull(word));
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
							// Если белый список получен, устанавливаем его
							if(!goodwords.empty()) alm->setGoodwords(goodwords);
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если пользовательские токены получены
					if(((value = env.get("utokens")) != nullptr) && (string(value).compare("-yes-") != 0)){
						// Список пользовательских токенов
						vector <wstring> tokens;
						// Выполняем извлечение пользовательских токенов
						alphabet.split(value, "|", tokens);
						// Если список токенов получен
						if(!tokens.empty()){
							// Если адрес скрипта получен
							if((value = env.get("utoken-script")) != nullptr){
								// Устанавливаем адрес скрипта
								alm->setUserTokenScript(value);
								// Переходим по всему списку токенов
								for(auto & item : tokens) alm->setUserToken(alphabet.convert(item));
							}
						}
					}
					// Список альтернативных букв
					map <wchar_t, wchar_t> altLetters;
					// Список альтернативных слов
					unordered_map <string, string> altWords;
					// Создаём словарь для работы спеллчекера
					dict_t dict(alm.get(), &alphabet, &tokenizer);
					// Устанавливаем адрес файла для логирования
					dict.setLogfile(env.get("log"));
					// Если количество ядер передано
					if(((value = env.get("threads")) != nullptr) && alphabet.isNumber(value)){
						// Устанавливаем количество потоков
						dict.setThreads(stoi(value));
					// Иначе устанавливаем 1 поток
					} else dict.setThreads(1);
					// Включаем режим отладки
					if(debug == 2) dict.setOption(dict_t::options_t::debug);
					// Если список пилотных слов передан
					if((value = env.get("pilots")) != nullptr){
						try {
							// Выполняем парсинг пилотных слов
							const json & pilots = json::parse(value);
							// Если пилотные слова получены
							if(!pilots.empty()){
								// Строка содержащее значение
								string text = "";
								// Переходим по всему списку пилотных слов
								for(auto & el : pilots.items()){
									// Получаем значение объекта
									text = el.value();
									// Добавляем пилотное слово
									if(!text.empty()) dict.setPilot(alphabet.convert(text).front());
								}
							}
						// Если возникает ошибка
						} catch(const exception & e) {
							// Выводим сообщение об ошибке
							print(alphabet.format("the JSON pilot words [%s] is broken\r\n", value), env.get("log"));
						}
					}
					// Если альтернативные буквы переданы
					if((value = env.get("alter")) != nullptr){
						try {
							// Выполняем парсинг альтернативные буквы
							const json & alters = json::parse(value);
							// Если альтернативные буквы получены
							if(!alters.empty()){
								// Строка содержащее значение
								string text = "";
								// Ключ и значение объекта
								wchar_t key = 0, value = 0;
								// Переходим по всему списку альтернативных букв
								for(auto & el : alters.items()){
									// Ключ объекта
									text = el.key();
									// Добавляем пилотное слово
									if(!text.empty()){
										// Получаем ключ объекта
										key = alphabet.convert(text).front();
										// Получаем значение объекта
										text = el.value();
										// Добавляем пилотное слово
										if(!text.empty()){
											// Получаем значение объекта
											value = alphabet.convert(text).front();
											// Если оба значения получены
											if((key > 0) && (value > 0)){
												// Устанавливаем альтернативные буквы
												dict.addAlt(key, value);
												// Запоминаем устанавливаемые альтернативные буквы
												altLetters.emplace(key, value);
											}
										}
									}
								}
							}
						// Если возникает ошибка
						} catch(const exception & e) {
							// Выводим сообщение об ошибке
							print(alphabet.format("the JSON alter letters [%s] is broken\r\n", value), env.get("log"));
						}
					}
					// Если файл с словами начинающихся всегда с заглавной буквы, передан
					if(((value = env.get("upwords")) != nullptr) && fsys_t::isfile(value)){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read upwords file", "Read upwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех альтернативных слов
							fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!word.empty()){
									// Устанавливаем слово, которое всегда должно начинаться с заглавной буквы
									dict.addUWord(word);
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += word.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					// Если каталог с словами начинающихся всегда с заглавной буквы, передан
					} else if(((value = env.get("upwords")) != nullptr) && fsys_t::isdir(value)) {
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем каталог для загрузки
							const string & path = realpath(value, nullptr);
							// Расширение файлов текстового корпуса
							const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read upwords file", "Read upwords file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Переходим по всему списку словарей в каталоге
							fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
								// Если отладка включена, выводим название файла
								if(debug > 0) pss.description(filename);
								// Выполняем загрузку файла словаря списка слов
								fsys_t::rfile2(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
									// Если слово получено
									if(!word.empty()){
										// Устанавливаем слово, которое всегда должно начинаться с заглавной буквы
										dict.addUWord(word);
										// Если отладка включена, выводим индикатор загрузки
										if(debug > 0){
											// Общий полученный размер данных
											size += word.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(dirSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									}
								});
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если файл с альтернативными словами указан
					if(((value = env.get("alters")) != nullptr) && (string(value).compare("-yes-") != 0)){
						/**
						 * Отлавливаем возможные ошибки
						 */
						try {
							// Позиция слова в списоке
							size_t pos = wstring::npos;
							// Параметры индикаторы процесса
							size_t size = 0, status = 0, rate = 0;
							// Запоминаем адрес файла для загрузки
							const string & filename = realpath(value, nullptr);
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Read alters file", "Read alters file, is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех альтернативных слов
							fsys_t::rfile(filename, [&](const string & line, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!line.empty()){
									// Получаем разделитель слов
									pos = line.find("\t");
									// Если разделитель найден
									if(pos != wstring::npos){
										// Добавляем альтернативный вариант
										altWords.emplace(line.substr(0, pos), line.substr(pos + 1));
									}
									// Если отладка включена, выводим индикатор загрузки
									if(debug > 0){
										// Общий полученный размер данных
										size += line.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(fileSize) * 100.0);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						/**
						 * Если происходит ошибка
						 */
						} catch(const exception & error) {
							/**
							 * Если включён режим отладки
							 */
							#if defined(DEBUG_MODE)
								// Выводим сообщение об ошибке
								print("error: %s\r\n", error.what());
							#endif
						}
					}
					// Если эмбеддинг передан
					if(((value = env.get("embedding")) != nullptr) && (string(value).compare("-yes-") != 0)){
						// Если это файл
						if(fsys_t::isfile(value)){
							// Получаем данные эмбеддинга
							string data = "";
							// Выполняем считывание всех альтернативных слов
							fsys_t::rfile(value, [&data](const string & line, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!line.empty()) data.append(line);
							});
							// Если блок данных получен, формируем эмбеддинг
							if(!data.empty()){
								try {
									// Выполняем парсинг эмбеддинга
									const json & embedding = json::parse(data);
									// Если эмбеддинг получен
									if(!embedding.empty()){
										// Получаем размер эмбеддинга
										const u_int size = (((value = env.get("embedding-size")) != nullptr) ? stoi(value) : embedding.size());
										// Устанавливаем эмбеддинг
										dict.setEmbedding(embedding, size);
									}
								// Если возникает ошибка
								} catch(const exception & e) {
									// Выводим сообщение об ошибке
									print(alphabet.format("the JSON embedding [%s] is broken\r\n", data.c_str()), env.get("log"));
								}
							}
						// Если это не файл
						} else {
							try {
								// Выполняем парсинг эмбеддинга
								const json & embedding = json::parse(value);
								// Если эмбеддинг получен
								if(!embedding.empty()){
									// Получаем размер эмбеддинга
									const u_int size = (((value = env.get("embedding-size")) != nullptr) ? stoi(value) : embedding.size());
									// Устанавливаем эмбеддинг
									dict.setEmbedding(embedding, size);
								}
							// Если возникает ошибка
							} catch(const exception & e) {
								// Выводим сообщение об ошибке
								print(alphabet.format("the JSON embedding [%s] is broken\r\n", value), env.get("log"));
							}
						}
					}
					// Устанавливаем дату словаря
					dict.setDate(time(nullptr));
					// Если код словаря передан
					if((value = env.get("bin-code")) != nullptr) dict.setCode(alphabet.convert(value));
					// Если название словаря передано
					if((value = env.get("bin-name")) != nullptr) dict.setName(alphabet.convert(value));
					// Если автор словаря передан
					if((value = env.get("bin-author")) != nullptr) dict.setAuthor(alphabet.convert(value));
					// Если тип лицензии словаря передан
					if((value = env.get("bin-lictype")) != nullptr) dict.setLictype(alphabet.convert(value));
					// Если текст лицензии словаря передан
					if((value = env.get("bin-lictext")) != nullptr) dict.setLictext(alphabet.convert(value));
					// Если контакты автора словаря переданы
					if((value = env.get("bin-contacts")) != nullptr) dict.setContacts(alphabet.convert(value));
					// Если копирайт словаря передан
					if((value = env.get("bin-copyright")) != nullptr) dict.setCopyright(alphabet.convert(value));
					// Получаем инфо-данные словаря
					auto vocabInfo = toolkit.getStatistic();
					// Если нужно пропрунить словарь, языковую модель или добавить список слов
					if((vocabInfo.first == 0) && (vocabInfo.second == 0) &&
					(env.is("vprune") || env.is("aprune") || env.is("r-words")) && env.is("r-vocab") && env.is("r-arpa")){
						// Если требуется загрузить файл словаря vocab
						if((value = env.get("r-vocab")) != nullptr){
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Запоминаем адрес файла
								const string & filename = realpath(value, nullptr);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(filename);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Read vocab", "Read vocab, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем загрузку файла словаря vocab
								toolkit.readVocab(filename, [debug, &pss](const string & filename, const u_short status) noexcept {
									// Если отладка включена, выводим имя файла
									if(debug > 0) pss.description(filename);
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						}
						// Если требуется загрузить файл словаря abbr
						if((value = env.get("r-abbr")) != nullptr){
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Запоминаем адрес файла
								const string & filename = realpath(value, nullptr);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(filename);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Read abbr", "Read abbr, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем загрузку файла суффиксов цифровых аббревиатур
								tokenizer.readSuffix(filename, [debug, &pss](const string & filename, const u_short status) noexcept {
									// Если отладка включена, выводим имя файла
									if(debug > 0) pss.description(filename);
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						}
						// Если требуется загрузить arpa
						if(((value = env.get("r-arpa")) != nullptr) && fsys_t::isfile(value)){
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Запоминаем адрес файла
								const string & filename = realpath(value, nullptr);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(filename);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Read arpa file", "Read arpa file, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем чтение arpa
								toolkit.readArpa(filename, [debug, &pss](const u_short status) noexcept {
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						}
						// Если нужно выполнить прунинг словаря
						if(env.is("vprune") && (env.is("vprune-wltf") || env.is("vprune-oc") || env.is("vprune-dc"))) {
							// Получаем порог встречаемости слова
							const size_t oc = (((value = env.get("vprune-oc")) != nullptr) && alphabet.isNumber(value) ? stoull(value) : 0);
							// Получаем порог количества документов, где встретилось слово
							const size_t dc = (((value = env.get("vprune-dc")) != nullptr) && alphabet.isNumber(value) ? stoull(value) : 0);
							// Получаем порог веса слова
							const double wltf = (((value = env.get("vprune-wltf")) != nullptr) && alphabet.isDecimal(value) ? stod(value) : 0.0);
							// Проверяем на правильность входных параметров
							if((wltf != 0.0) || (oc > 0) || (dc > 0)){
								// Если отладка включена, выводим индикатор прунинга
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем заголовки прогресс-бара
									pss.title("Prune vocab", "Prune vocab, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем прунинг словаря
								toolkit.pruneVocab(wltf, oc, dc, [debug, &pss](const u_short status) noexcept {
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							// Сообщаем, что порог передан неверный
							} else print("threshold is broken\r\n", env.get("log"));
						}
						// Если нужно выполнить прунинг arpa
						if(env.is("aprune")){
							// Максимальный размер n-граммы
							u_short size = 0;
							// Коэффициент прунинга
							double prune = 0.0;
							// Если параметр максимального размера n-граммы для прунинга получен
							if((value = env.get("aprune-max-gram")) != nullptr) size = stoi(value);
							// Если параметр прунинга получен
							if((value = env.get("aprune-threshold")) != nullptr) prune = stod(value);
							// Если параметры получены
							if((size > 0) && (prune != 0.0)){
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем заголовки прогресс-бара
									pss.title("Prune arpa", "Prune arpa, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем прунинг
								toolkit.prune(prune, size, [debug, &pss](const u_short status) noexcept {
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							}
						}
						// Если требуется загрузить файл списка слов
						if(((value = env.get("r-words")) != nullptr) && fsys_t::isfile(value)){
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Параметры индикаторы процесса
								size_t size = 0, status = 0, rate = 0;
								// Запоминаем адрес файла
								const string & filename = realpath(value, nullptr);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(filename);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Read words file", "Read words file, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем загрузку файла словаря списка слов
								fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
									// Если слово получено
									if(!word.empty()){
										// Добавляем слово в словарь
										toolkit.addWord(alphabet.convert(word));
										// Если отладка включена, выводим индикатор загрузки
										if(debug > 0){
											// Общий полученный размер данных
											size += word.size();
											// Подсчитываем статус выполнения
											status = u_short(size / double(fileSize) * 100.0);
											// Если процентное соотношение изменилось
											if(rate != status){
												// Запоминаем текущее процентное соотношение
												rate = status;
												// Отображаем ход процесса
												switch(debug){
													case 1: pss.update(status); break;
													case 2: pss.status(status); break;
												}
											}
										}
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						// Если требуется загрузить каталог списка слов
						} else if(((value = env.get("r-words")) != nullptr) && fsys_t::isdir(value)) {
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Параметры индикаторы процесса
								size_t size = 0, status = 0, rate = 0, index = 0;
								// Запоминаем каталог для загрузки
								const string & path = realpath(value, nullptr);
								// Расширение файлов текстового корпуса
								const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем заголовки прогресс-бара
									pss.title("Read words file", "Read words file, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Переходим по всему списку словарей в каталоге
								fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
									// Если отладка включена, выводим название файла
									if(debug > 0) pss.description(filename);
									// Выполняем загрузку файла словаря списка слов
									fsys_t::rfile2(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
										// Если слово получено
										if(!word.empty()){
											// Добавляем слово в словарь
											toolkit.addWord(alphabet.convert(word), 0, index);
											// Если отладка включена, выводим индикатор загрузки
											if(debug > 0){
												// Общий полученный размер данных
												size += word.size();
												// Подсчитываем статус выполнения
												status = u_short(size / double(dirSize) * 100.0);
												// Если процентное соотношение изменилось
												if(rate != status){
													// Запоминаем текущее процентное соотношение
													rate = status;
													// Отображаем ход процесса
													switch(debug){
														case 1: pss.update(status); break;
														case 2: pss.status(status); break;
													}
												}
											}
										}
									});
									// Увеличиваем количество обработанных документов
									index++;
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						}
						// Получаем инфо-данные словаря
						vocabInfo = toolkit.getStatistic();
					}
					// Если словарь существует
					if((vocabInfo.first > 0) && (vocabInfo.second > 0)){
						// Временное слово для извлечения
						word_t tmp = L"";
						// Параметры индикаторы процесса
						size_t index = 0, status = 0, rate = 0;
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read words", "Read words, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Устанавливаем данные словаря
						dict.setAdCw(vocabInfo.second, vocabInfo.first);
						// Выполняем извлечение слов словаря
						toolkit.words([&](const wstring & word, const size_t idw, const size_t oc, const size_t dc, const size_t count){
							// Формируем слово
							tmp = word;
							// Устанавливаем метаданные слова
							tmp.setmeta(dc, oc);
							// Добавляем слово в словарь
							dict.add(alm->getIdw(tmp), tmp);
							// Если отладка включена
							if(debug > 0){
								// Общий полученный размер данных
								index++;
								// Подсчитываем статус выполнения
								status = u_short(index / double(count) * 100.0);
								// Если процентное соотношение изменилось
								if(rate != status){
									// Запоминаем текущее процентное соотношение
									rate = status;
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								}
							}
							// Сообщаем, что все хорошо
							return true;
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read language model", "Read language model, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Устанавливаем размер N-граммы
						alm->setSize(toolkit.getSize());
						// Выполняем извлечение языковой модели
						toolkit.saveArpa([&](const vector <char> & buffer, const u_short status){
							// Устанавливаем данные языковой модели
							alm->setBin(buffer);
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						}, true);
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Train dictionary", "Train dictionary, is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем обучение словаря
						dict.train([debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Значит туллкит не заполнен
					} else if(env.is("r-vocab") && env.is("r-arpa")) {
						// Если требуется загрузить файл словаря vocab
						if(((value = env.get("r-vocab")) != nullptr) && fsys_t::isfile(value)){
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Запоминаем адрес файла
								const string & filename = realpath(value, nullptr);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(filename);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Read vocab file", "Read vocab file, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем загрузку файла словаря vocab
								dict.readVocab(filename, [debug, &pss](const u_short status) noexcept {
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем заголовки прогресс-бара
									pss.title("Train dictionary", "Train dictionary, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем обучение
								dict.train([debug, &pss](const u_short status) noexcept {
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						}
						// Если требуется загрузить arpa
						if(((value = env.get("r-arpa")) != nullptr) && fsys_t::isfile(value)){
							/**
							 * Отлавливаем возможные ошибки
							 */
							try {
								// Запоминаем адрес файла
								const string & filename = realpath(value, nullptr);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Очищаем предыдущий прогресс-бар
									pss.clear();
									// Устанавливаем название файла
									pss.description(filename);
									// Устанавливаем заголовки прогресс-бара
									pss.title("Read arpa file", "Read arpa file, is done");
									// Выводим индикатор прогресс-бара
									switch(debug){
										case 1: pss.update(); break;
										case 2: pss.status(); break;
									}
								}
								// Выполняем чтение arpa
								alm->read(filename, [debug, &pss](const u_short status) noexcept {
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								});
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							/**
							 * Если происходит ошибка
							 */
							} catch(const exception & error) {
								/**
								 * Если включён режим отладки
								 */
								#if defined(DEBUG_MODE)
									// Выводим сообщение об ошибке
									print("error: %s\r\n", error.what());
								#endif
							}
						}
					// Сообщаем, что файлы для загрузки не найдены
					} else print("file vocab or arpa not found\r\n", env.get("log"));
					// Если словарь получен и обучен
					if(!dict.empty()){
						// Объект бинарного контейнера
						ascb_t ascb(binDictFile, ((value = env.get("bin-password")) != nullptr ? value : ""), env.get("log"));
						// Устанавливаем словарь
						ascb.setDict(&dict);
						// Устанавливаем режим отладки
						ascb.setDebug(debug);
						// Устанавливаем языковую модель
						ascb.setAlm(alm.get());
						// Устанавливаем прогресс-бар
						ascb.setProgress(&pss);
						// Устанавливаем алфавит
						ascb.setAlphabet(&alphabet);
						// Устанавливаем токенизатор
						ascb.setTokenizer(&tokenizer);
						// Устанавливаем тип языковой модели
						ascb.setALMvType(env.is("alm2"));
						// Устанавливаем локаль приложения
						if((value = env.get("locale")) != nullptr) ascb.setLocale(value);
						// Если размер шифрования передан
						if((value = env.get("bin-aes")) != nullptr){
							// Если размер шифрования получен
							switch(stoi(value)){
								// Если это 192-х битное шифрование
								case 192: ascb.setAES(aspl_t::types_t::aes192); break;
								// Если это 256-и битное шифрование
								case 256: ascb.setAES(aspl_t::types_t::aes256); break;
								// Если это 128-и битное шифрование
								default: ascb.setAES(aspl_t::types_t::aes128);
							}
						// Устанавливаем тип шифрования по умолчанию
						} else ascb.setAES(aspl_t::types_t::aes128);
						// Если список альтернативных букв существуют
						if(!altLetters.empty()){
							// Переходим по всему списку альтернативных букв
							for(auto & item : altLetters) ascb.addAlt(item.first, item.second);
							// Если список альтернативных слов загружен
							if(!altWords.empty()){
								// Переходим по всему списку альтернативных слов
								for(auto & item : altWords) ascb.addAlt(item.first, item.second);
							}
						}
						// Выполняем инициализацию контейнера
						ascb.init();
						// Выполняем запись данных в файл контейнера
						ascb.write();
					}
				}
			}
			// Если режим отладки включён
			if(debug > 0){
				// Получаем диапазон времени
				auto dimension = pss.dimension(chrono::duration_cast <chrono::seconds> (chrono::system_clock::now() - timeShifting).count());
				// Выводим результат
				print(alphabet.format("work time shifting: %lld %s\r\n", dimension.first, dimension.second.c_str()), env.get("log"), alphabet_t::log_t::info);
			}
		// Сообщаем, что файл алфавита не найден
		} else print("file alphabet not found\r\n", env.get("log"));
	}
	// Успешно выходим
	return 0;
}
