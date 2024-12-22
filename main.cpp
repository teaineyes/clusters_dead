#define CURL_STATICLIB
#include <curl\curl.h>
#include <iostream>
#include <regex>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <random>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

std::string generateReportText(const std::string& user, const std::string& user_id, const std::string& proof_link, const std::string& filename) {
    std::vector<std::string> reportTexts;
    std::ifstream file(filename.c_str());
    if (!file) {
        throw std::runtime_error("[ CLUSTERS * read/error ] -> Unable to open the file. ");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            reportTexts.push_back(line);
        }
    }
    file.close();

    if (reportTexts.empty()) {
        throw std::runtime_error("[ CLUSTERS * read/error ] -> No report texts found. ");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, reportTexts.size() - 1);
    std::string selectedText = reportTexts[dist(gen)];

    size_t pos;
    while ((pos = selectedText.find("{user}")) != std::string::npos) {
        selectedText.replace(pos, 6, user);
    }
    while ((pos = selectedText.find("{user_id}")) != std::string::npos) {
        selectedText.replace(pos, 9, user_id);
    }
    while ((pos = selectedText.find("{proof_link}")) != std::string::npos) {
        selectedText.replace(pos, 12, proof_link);
    }
    return selectedText;
}

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string generateRandomPhoneNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 9);

    const std::vector<std::pair<std::string, std::vector<std::string>>> countryOperators = {
        {"+1", {"201", "202", "203", "204", "205", "206", "207", "208", "209", "210",
                  "212", "213", "214", "215", "216", "217", "218", "219", "224", "225"}},
        {"+44", {"740", "750", "760", "770", "780", "790", "800", "810", "820", "830",
                  "840", "850", "860", "870", "880", "890"}},
        {"+7",  {"900", "901", "902", "903", "904", "905", "906", "907", "908", "909", 
                 "911", "912", "913", "914", "915", "916", "917", "918", "919", "920", 
                 "921", "922", "923", "924", "925", "926", "927", "928", "929"}},
        {"+380", {"50", "63", "66", "67", "68", "73", "91", "92", "93", "94", "95", 
                  "96", "97", "98", "99"}} 
    };

    std::uniform_int_distribution<> countryDist(0, countryOperators.size() - 1);
    const auto& selectedCountry = countryOperators[countryDist(gen)];
    const std::string& countryCode = selectedCountry.first;

    std::uniform_int_distribution<> operatorDist(0, selectedCountry.second.size() - 1);
    const std::string& operatorCode = selectedCountry.second[operatorDist(gen)];

    std::string phoneNumber = countryCode + operatorCode;

    int remainingDigits = (countryCode == "+1") ? 7 : (countryCode == "+44") ? 7 : 7; 

    for (int i = 0; i < remainingDigits; ++i) {
        phoneNumber += std::to_string(dist(gen));
    }

    return phoneNumber;
}

std::string generateFullName(const std::string& phoneNumber = "") {
    std::vector<std::string> englishFirstNames = {
        "John", "Alice", "Michael", "Sarah", "David", "Emily", "James", "Sophia", "William", "Olivia",
        "Ethan", "Isabella", "Benjamin", "Amelia", "Lucas", "Mia", "Henry", "Chloe", "Alexander", "Charlotte",
        "Jackson", "Victoria", "Owen", "Grace", "Jack", "Harper", "Samuel", "Avery", "Matthew", "Lily",
        "Aiden", "Ella", "Jackson", "Scarlett", "Gabriel", "Leah", "Liam", "Madison", "Elijah", "Hannah",
        "Sebastian", "Lillian", "Zoe", "Maxwell", "Addison", "Isaiah", "Aria", "Nathaniel", "Clara", "Zane",
        "Mason", "Ella", "Nolan", "Victoria", "Maverick", "Zachary", "Ruby", "Avery", "Caroline", "Wyatt",
        "Caleb", "Harrison", "Eleanor", "Brianna", "Chase", "Savannah", "Noah", "Grace", "Scarlett", "Jack",
        "Theo", "Layla", "Julian", "Penelope", "Eli", "Aurora", "Miles", "Hazel", "Calvin", "Ivy",
        "Andrew", "Eliza", "Ezra", "Violet", "Aaron", "Stella", "Joshua", "Natalie", "Ryan", "Autumn",
        "Nathan", "Bella", "Adam", "Lydia", "Cole", "Willow", "Declan", "Eva", "Dylan", "Paige",
        "Hunter", "Faith", "Connor", "Elena", "Brandon", "Aurora", "Parker", "Brooke", "Tristan", "Jasmine"
    };

    std::vector<std::string> englishLastNames = {
        "Smith", "Johnson", "Williams", "Brown", "Jones", "Miller", "Davis", "Garcia", "Martinez", "Hernandez",
        "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas", "Taylor", "Moore", "Jackson", "Martin", "Lee",
        "Perez", "White", "Harris", "Clark", "Lewis", "Roberts", "Walker", "Young", "Allen", "King",
        "Scott", "Adams", "Baker", "Nelson", "Hill", "Ramirez", "Campbell", "Mitchell", "Evans", "Green",
        "Graham", "Sanchez", "Reed", "Morgan", "Bishop", "Hughes", "Bryant", "Alexander", "Jameson", "Davidson",
        "Shaw", "Ross", "Cooper", "Foster", "Phillips", "Edwards", "Bailey", "Simpson", "Burns", "Curtis",
        "Hunt", "Lawrence", "Jenkins", "Bennett", "Cameron", "Douglas", "Grant", "Reynolds", "Fisher", "Barnes",
        "Cross", "Holland", "McCarthy", "Griffin", "Collins", "Webb", "Harrison", "Stewart", "Cook", "Paterson",
        "Myers", "Perry", "Jordan", "Fletcher", "Stone", "Bradley", "Knight", "Murray", "Harvey", "Freeman",
        "Wright", "Chapman", "Fox", "Ellis", "Armstrong", "Crawford", "Dean", "Watson", "Peters", "Murphy"
    };

    std::vector<std::string> russianFirstNames = {
        "Ivan", "Dmitry", "Alexei", "Sergey", "Mikhail", "Viktor", "Nikolai", "Yuri", "Andrei", "Pavel", 
        "Maxim", "Alexandr", "Leonid", "Roman", "Kirill", "Igor", "Oleg", "Vadim", "Artur", "Denis", 
        "Vladislav", "Evgeny", "Petr", "Gennady", "Boris", "Vyacheslav", "Stanislav", "Valery", "Konstantin", 
        "Nikita", "Yaroslav", "Maksim", "Vsevolod", "Yegor", "Semyon", "Artem", "Aleksey", "Igor", "Vitali", 
        "Petr", "Oleg", "Dmitriy", "Tamerlan", "Makar", "Kamil", "Arseniy", "Vlad", "Mihail", "Yegor", 
        "Nikolay", "Borys", "Roman", "Valery", "Artyom", "Vyacheslav", "Pavel", "Anton", "Alexei", "Daniil", 
        "Mikhail", "Egor", "Semen", "Dmitri", "Lev", "Kirill", "Svetoslav", "Filipp", "Stanislav", "Fedor", 
        "Ilya", "Yegor", "Ruslan", "Aleksei", "Sergey", "Valentin", "Vladimir", "Maksim", "Dmitriy", "Roman", 
        "Yuriy", "Alexander", "Timofey", "Anton", "Aleksei", "Eugene", "Boris", "Yuri", "Vsevolod"
    };

    std::vector<std::string> russianLastNames = {
        "Ivanov", "Petrov", "Sidorov", "Smirnov", "Kuznetsov", "Popov", "Vasiliev", "Novikov", "Morozov", "Fedorov",
        "Orlov", "Tarasov", "Karpov", "Semenov", "Pavlov", "Mikhailov", "Dmitriev", "Belyaev", "Zaharov", "Krylov",
        "Frolov", "Gorbachov", "Borisov", "Chernyshev", "Tikhonov", "Korolev", "Kachur", "Shishkin", "Vasilenko",
        "Nikitin", "Panteleev", "Zhuravlev", "Rogozin", "Kulikov", "Lavrov", "Makarov", "Aleksandrov", "Karasev",
        "Varnavsky", "Yermakov", "Shchukin", "Romanov", "Bocharnikov", "Galkin", "Zhukov", "Sergiev", "Salnikov",
        "Matveev", "Savelyev", "Kovalev", "Pirogov", "Lopatin", "Dmitriev", "Morozov", "Shirokov", "Belyaev", 
        "Strakhov", "Maksimov", "Krasnov", "Belyakov", "Kuzmin", "Likhachev", "Sharapov", "Goncharov", "Tregubov", 
        "Varlamov", "Andreev", "Markov", "Romanov", "Shustov", "Grigoryev", "Shvetsov", "Kharlov", "Bessarabov", 
        "Skorik", "Klochkov", "Solntsev", "Koryakov", "Danilov", "Shturman", "Goncharov", "Vasiliev", "Golovin"
    };

    std::vector<std::string> ukrainianFirstNames = {
        "Bohdan", "Taras", "Serhiy", "Mykola", "Yevhen", "Oleh", "Andriy", "Vladyslav", "Danylo", "Artem",
        "Viktor", "Vitaliy", "Roman", "Oleksandr", "Ihor", "Yuriy", "Vladislav", "Maxym", "Pavlo", "Sergiy",
        "Vadym", "Leonid", "Borys", "Igor", "Sergey", "Tymur", "Denys", "Ostap", "Vsevolod", "Anatoliy",
        "Ruslan", "Kyrylo", "Dmytro", "Ivan", "Artem", "Maksym", "Viktoriya", "Diana", "Valeriya", "Ksenia",
        "Volodymyr", "Svyatoslav", "Olexandr", "Serhiy", "Yevheniy", "Nazar", "Petr", "Yuriy", "Roman", 
        "Andriy", "Tymofiy", "Yaroslav", "Vladislav", "Vlad", "Maksym", "Myroslav", "Olexiy", "Taras", 
        "Mikhail", "Ostap", "Mykola", "Aleksey", "Semen", "Petr", "Dmitriy", "Nikolay", "Yuriy", "Dmitro", 
        "Sergey", "Igor", "Evgen", "Mikhail", "Vasyl", "Borys", "Danylo", "Bohdan", "Vitaliy", "Roman",
        "Yevhen", "Igor", "Artem", "Maksym", "Tymur", "Iryna", "Alina", "Kateryna", "Olga", "Anastasia"
    };

    std::vector<std::string> ukrainianLastNames = {
        "Shevchenko", "Kovalenko", "Bondarenko", "Tkachenko", "Melnyk", "Belyk", "Marchenko", "Petriv", "Savchuk", "Hrytsenko",
        "Zhyvotkov", "Dubovik", "Lysenko", "Bohdanov", "Kulyk", "Shulha", "Krut", "Kravchuk", "Kushnir", "Shpak",
        "Solodovnyk", "Yushchenko", "Borysov", "Filipchuk", "Kholod", "Klymenko", "Chorny", "Olenchuk", "Klimenko", 
        "Chernenko", "Romanenko", "Tarasenko", "Hlushko", "Kovbas", "Chernov", "Taran", "Varenytsia", "Petryshyn", "Fedoriv",
        "Voloshyn", "Kovach", "Chizh", "Krasivskyi", "Kozak", "Horbenko", "Kozel", "Baran", "Dudchenko", "Krivoruchko", 
        "Solohub", "Kovbasa", "Holub", "Rudyk", "Stepanenko", "Panteleev", "Bazhuk", "Malyk", "Cherevko", "Popovych",
        "Kovach", "Mykhailov", "Yablonski", "Fedorov", "Kolos", "Sushchenko", "Tyschenko", "Shyshkova", "Vernigor", "Barynya"
    };

    std::string countryCode = phoneNumber.substr(0, 3);
    
    std::vector<std::string> firstNames;
    std::vector<std::string> lastNames;

    if (countryCode == "+1") { 
        firstNames = englishFirstNames;
        lastNames = englishLastNames;
    } else if (countryCode == "+44") {
        firstNames = englishFirstNames;
        lastNames = englishLastNames;
    } else if (countryCode == "+7") {
        firstNames = russianFirstNames;
        lastNames = russianLastNames;
    } else if (countryCode == "+380") {
        firstNames = ukrainianFirstNames;
        lastNames = ukrainianLastNames;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> firstNameDist(0, firstNames.size() - 1);
    std::uniform_int_distribution<> lastNameDist(0, lastNames.size() - 1);

    std::string firstName = firstNames[firstNameDist(gen)];
    std::string lastName = lastNames[lastNameDist(gen)];

    return firstName + " " + lastName;
}

std::string generateRandomEmail() {
    const std::vector<std::string> domains = { "gmail.com", "mail.ru", "yandex.ru"};
    const std::string chars = "abcdefghijklmnopqrstuvwxyz0123456789";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_char(0, chars.size() - 1);
    std::uniform_int_distribution<> dist_domain(0, domains.size() - 1);
    std::uniform_int_distribution<> dist_len(5, 10);

    int name_len = dist_len(gen);

    std::string name;
    for (int i = 0; i < name_len; ++i) {
        name += chars[dist_char(gen)];
    }

    return name + "@" + domains[dist_domain(gen)];
}

std::vector<std::string> getProxiesFromWebsite(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "[ CLUSTERS *X* preload/proxy ] -> Load proxy failed error. " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    std::vector<std::string> proxies;
    std::regex proxy_regex(R"((http|https)://(\w+:\w+@)?([\d\w\.-]+):(\d+))");
    auto words_begin = std::sregex_iterator(readBuffer.begin(), readBuffer.end(), proxy_regex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        proxies.push_back(i->str());
    }

    return proxies;
}

std::vector<std::string> userAgents = {
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Firefox/89.0",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.85 Safari/537.36",
    "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:89.0) Gecko/20100101 Firefox/89.0",
    "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:35.0) Gecko/20100101 Firefox/35.0"
};

void sendComplaint(const std::string& proxy, const std::string& message, const std::string& phone, const std::string& email, const std::string& fullname) {

    const char* GREEN = "\033[32m";
    const char* RESET = "\033[0m";

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());

        int randomIndex = rand() % userAgents.size();
        curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgents[randomIndex].c_str());

        curl_easy_setopt(curl, CURLOPT_URL, "https://telegram.org/support");

        std::string escapedMessage = curl_easy_escape(curl, message.c_str(), message.length());
        std::string escapedFullname = curl_easy_escape(curl, fullname.c_str(), fullname.length());
        std::string escapedEmail = curl_easy_escape(curl, email.c_str(), email.length());
        std::string escapedPhone = curl_easy_escape(curl, phone.c_str(), phone.length());
        std::string payload = "message=" + escapedMessage +
            "&legal_name=" + escapedFullname +
            "&email=" + escapedEmail +
            "&phone=" + escapedPhone +
            "&setln=";

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << GREEN <<  "[ CLUSTERS * request/status ] ->  Request failed -> " << curl_easy_strerror(res) << RESET << std::endl;
        }
        else {
            std::cout << "[ CLUSTERS * request/status ] -> Request done. " << std::endl;
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
    else {
        std::cerr << "[ CLUSTERS * request/error ] -> Initialization failed." << std::endl;
    }

    curl_global_cleanup();
}

void asyncSendComplaint(const std::string& proxy, const std::string& text, const std::string& number, const std::string& email, const std::string& fullname) {
    std::async(std::launch::async, sendComplaint, proxy, text, number, email, fullname);
}

int main() {
    const char* RED = "\033[31m";
    const char* RESET = "\033[0m"; 
    const char* BLUE = "\033[34m";
    std::string logotype = R"(
                                                                                         
      .,~:::::`::.                   `::                                :::::::-.                           :: 
    ,;;;'````' ;;;                    ;;                                 ;;,   `';,                         ;;;
    [[[        [[[ ,c  ,  ,cc[[[cc.=[[[[[[.,cc[[[cc.=,,[[==,cc[[[cc.     `[[     [[,cc[[[cc.  ,ccc,    ,c[[[cc,
    $$$        $$'$$'  $$$$$$____     $$   $$$___--'`$$$"``$$$____        $$,    $$$$$___--' $$$cc$$$ $$""""Y$$
    `88bo,__,o,\8o888   888.     88,  88,  88b    ,o,888    .     88,     888_,o8P'88b    ,o,888   88888o,,od8P
      "YUMMMMMP"MM;"YUM" MP"YUMMMMP"  MMM   "YUMMMMP""MM,   "YUMMMMP"     MMMMP"`   "YUMMMMP" "YUM" MP"YUMMMP"           
                        
                        </ Developer -> Team Enclosure / Version -> Alpha / Build -> 4a />
                                </ Changelog -> Generating Legal Names updated -> 
                                     Added +7 +380 numbers and operators -> 

    )";

    std::vector<std::string> proxies = getProxiesFromWebsite("YOUR OWN PROXY URL");
    std::cout << RED <<  logotype << RESET << std::endl;
    std::string user;
    std::cout << BLUE << "[ CLUSTERS <- preload/username ] ->  " << RESET;
    std::cin >> user;
    std::cout << BLUE << "[ CLUSTERS <- preload/file ] -> Enter filename -> " << RESET;
    std::string filename;
    std::cin >> filename;
    std::cout << BLUE << "[ CLUSTERS <- preload/user_id ] -> " << RESET;
    std::string user_id;
    std::cin >> user_id;
    std::cout << BLUE << "[ CLUSTERS <- preload/proof_link ] -> " << RESET;
    std::string proof_link;
    std::cin >> proof_link;
    int reports = 0;
    system("cls");


    if (proxies.empty()) {
        std::cerr << "[ CLUSTERS -> preload/error ] -> No proxies found! Check the link. " << std::endl;
        return 1;
    }

    while (true) {
        std::cout << RED << logotype << RESET << std::endl;
        std::string report = generateReportText(user, user_id, proof_link, filename);
        std::cout << "[ CLUSTERS <- preload/text ] <- " + report << std::endl;
        std::string fullname = generateFullName();
        std::cout << "[ CLUSTERS <- preload/legalname ] <- " + fullname << std::endl;
        std::string number = generateRandomPhoneNumber();
        std::cout << "[ CLUSTERS <- preload/phone ] <- " + number << std::endl;
        std::string email = generateRandomEmail();
        std::cout << "[ CLUSTERS <- preload/email ] <- " + email << std::endl;
        std::string proxy = proxies[rand() % proxies.size()];
        std::cout << "[ CLUSTERS <- preload/proxy ] <- " + proxy << std::endl;
        std::cout << "[ CLUSTERS <- preload/user_id ] <- " + user_id << std::endl;
        std::cout << "[ CLUSTERS <- preload/proof_link ] <- " + proof_link << std::endl;
        std::cout << "[ CLUSTERS -> reports/done ] -> " + std::to_string(reports) << std::endl;
        asyncSendComplaint(proxy, report, number, email, fullname);
        system("cls");
        reports += 1;
    }

    return 0;
}










