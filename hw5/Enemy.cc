#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Enemy.h"
#include "Gallery.h"

static std::string NAME_KEY = "Name";
static std::string LINK_KEY_PREFIX = "Link";

static bool isBlank(std::string s){
    for(char c : s){
        if(!isspace(c)){
            return false;
        }
    }
    return true;
}

static bool isAlphaNum(std::string s){
    for(char c : s){
        if(!isalnum(c)){
            return false;
        }
    }
    return true;
}

static std::string removeLeadingSpaces(std::string s){
    std::string r = s;
    for(char c : s){
        if(isspace(c)){
            r = r.substr(1,sizeof(r));
        }
        else{
            return r;
        }
    }
    return r;
}

static std::string removeTrailingSpaces(std::string s){
    std::string r = s;
    for(int i = s.size()-1; i >= 0; i--){
        if(isspace(s[i])){
            r.pop_back();
        }
        else{
            break;
        }
    }
    return r;
}

static std::string formatEnemy(std::string key, std::string val, size_t maxsize){
    std::string s;
    s+= key;
    for(size_t i = key.length(); i <= maxsize; i++)
        s+= ' ';
    s+= val;
    return s;
}
    std::vector<std::string> keys, values, allowedkeys;
    bool nameflag = true, otherflag = true, linkflag = true;




    Enemy::Enemy(std::string keyfile){
        std::string line;
        std::ifstream input(keyfile);
        if(input.fail()){
            throw std::runtime_error("Enemy: Could not find file: " + keyfile + '\n');
        }
        while(getline(input, line)){
            line = removeTrailingSpaces(line);
            if(!isAlphaNum(line)) {
                throw std::runtime_error("Enemy: Non Alpha-Numeric key: \"" + line + "\" in key file " + keyfile + '\n');
            }
            allowedkeys.push_back(line);
        }
    }



    bool Enemy::read(std::istream &in){
        clear();
        std::string line;
        while(getline(in, line)){
            //todo: check if the input failed
            removeTrailingSpaces(line);
            if(isBlank(line)){                           //if line is empty after whitespace removed, line is blank
                if(!std::count(keys.begin(), keys.end(), NAME_KEY)){
                    throw std::runtime_error("Enemy: missing name key!\n");
                }
                return true;
            }
            std::string word;

            if(!isspace(line[0])){                      //if line contains a key
                std::stringstream wordDivider(line);
                wordDivider >> word;
                if(allowedkeys.empty()){
                    if(!isAlphaNum(word)){
                        throw std::runtime_error("Enemy: non-alphanumeric key \"" + word + "\"\n");
                    }
                    keys.push_back(word);
                }                                        //todo: error if not in supplied keyfile
                else if(std::count(allowedkeys.begin(), allowedkeys.end(), word)){
//                    std::cout << word;
                    keys.push_back(word);
                }
                else{       //invalid key
                    throw std::runtime_error("Enemy: key not contained in keyfile: \"" + word + "\"\n");                                    //todo: error
                }
                line = line.substr(word.length(), line.length());
                line = removeTrailingSpaces(line);
                line = removeLeadingSpaces(line);
                values.push_back(line);
            }
            else{

                line = removeTrailingSpaces(line);
                line = removeLeadingSpaces(line);
                if (values.back().empty()) {
                	values.back() = line;
                } else {
					values.back() += " " + line;
				}
            }

        }
        // ensure we were able to at least read the Name field
        if (keys.empty()) {
        	return false;
        } else if(!std::count(keys.begin(), keys.end(), NAME_KEY)){
			throw std::runtime_error("Enemy: missing name key!\n");
		} else {
        	return true;
        }
    }

    void Enemy::write(std::ostream &out) const {
        size_t maxsize = 0;
        for(size_t q = 0; q < keys.size(); q++){
            if(keys.at(q).size() > maxsize){
                //allowed size based on flags
                if(keys.at(q).compare("Name") == 0) {
                    if(nameflag == true) {
                        maxsize = keys.at(q).size();
                    }
                }
                else if(keys.at(q).substr(0,4).compare("Link") == 0){
                    if(linkflag == true) {
                        maxsize = keys.at(q).size();
                    }
                }
                else if(otherflag == true){
                    maxsize = keys.at(q).size();
                }

            }
        }
        if(nameflag){
            for(size_t i = 0; i < keys.size(); i++) {
                if (keys.at(i).compare("Name") == 0) {
                    out << formatEnemy(keys.at(i), values.at(i) , maxsize) << '\n';
                }
            }
        }
        if(otherflag){
            for(size_t i = 0; i < keys.size(); i++) {
                if (keys.at(i).compare("Name") != 0 && keys.at(i).substr(0, 4).compare("Link") != 0) {
                    out << formatEnemy(keys.at(i), values.at(i) , maxsize) << '\n';
                }
            }
        }
        if(linkflag){
            for(size_t i = 0; i < keys.size(); i++){
                if(keys.at(i).substr(0,4).compare("Link") == 0){
                    out << formatEnemy(keys.at(i), values.at(i) , maxsize) << '\n';
                }
            }
        }
    }
    void Enemy::write(std::string filename) const {
        std::ofstream file;
        file.open(filename);
        if(file.bad()){
            throw std::runtime_error("Enemy: Failed to open file " + filename);
        }
        Enemy::write(file);
        file.close();
    }
    std::string Enemy::field(std::string key) const {
        if(!std::count(keys.begin(), keys.end(), key)){
            throw std::range_error("Enemy: key \"" + key + "\" not contained in this Enemy");
        }
        return values.at(std::distance(keys.begin(), std::find(keys.begin(), keys.end(), key)));
    }
    void Enemy::show_name(bool b){
        nameflag = b;
    }
    void Enemy::show_other(bool b){
        otherflag = b;
    }
    void Enemy::show_link(bool b){
        linkflag = b;
    }
    void Enemy::clear(){
        keys.clear();
        values.clear();
        gallery = nullptr;
    }
    size_t Enemy::size() const {
        return keys.size();
    }
    bool Enemy::empty() const {
    	return keys.size() == 0;
    }

	Enemy* Enemy::link(std::string relation) const {
    	if (gallery == nullptr) {
    		throw std::runtime_error(field(NAME_KEY) + " is not part of a gallery");
    	} else {
    		try {
				std::string linkName = field(LINK_KEY_PREFIX + relation);
				for (size_t i = 0; i < gallery->size(); i++) {
					Enemy* e = gallery->get(i);
					std::string enemyName = e->field(NAME_KEY);	// should always work
					if (linkName == enemyName) {
						return e;
					}
				}
				throw std::runtime_error(relation + " enemy named " + linkName + " not found");
    		} catch (std::range_error& err) {
				throw std::runtime_error(relation + " link not found");
    		}
    	}
    }

	void Enemy::setGallery(Gallery* galleryIn) {
		gallery = galleryIn;
	}


std::ostream &operator<<(std::ostream &out, Enemy e) {
    e.write(out);
    return out;
}

