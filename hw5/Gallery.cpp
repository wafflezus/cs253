//
// Created by Dad on 11/1/19.
//

#include "Gallery.h"
#include <stdexcept>
#include <fstream>

Gallery::Gallery(const string &s1, const string &s2)
: Gallery(std::vector<std::string>{ s1, s2 })
{
}

Gallery::Gallery(const string &s1, const string &s2, const string &s3)
: Gallery(std::vector<std::string>{ s1, s2, s3 })
{
}

Gallery::Gallery(const string &s1, const string &s2, const string &s3, const string &s4)
: Gallery(std::vector<std::string>{ s1, s2, s3, s4 })
{
}

Gallery::Gallery(const string &s1, const string &s2, const string &s3, const string &s4, const string &s5)
: Gallery(std::vector<std::string>{ s1, s2, s3, s4, s5 })
{
}

Gallery::Gallery(const string &s1, const string &s2, const string &s3, const string &s4, const string &s5, const string &s6)
: Gallery(std::vector<std::string>{ s1, s2, s3, s4, s5, s6 })
{
}

static std::string findKeyFileName(std::vector<std::string>& files) {
	for (size_t i = 0; i < files.size(); i++) {
		std::string keyFileName = files[i];
		try {
			// try to parse the file as a key file
			Enemy e(keyFileName);
			// if it worked, remove the file name from the files list and return it
			files.erase(files.begin() + i, files.begin() + i + 1);
			return keyFileName;
		} catch (std::runtime_error &err) {
			// not a key file, so keep going.
		}
	}
	// we didn't find a key file so we just return and empty string file name
	return "";
}

Gallery::Gallery(std::vector<std::string> files) {
	std::string keyFileName = findKeyFileName(files);
	if (keyFileName.empty()) {
		//we checked all the files and did not find a key file.  Kaboom!
		throw std::runtime_error("Missing key file.");
	} else {
		//load the enemies.
		for (size_t j = 0; j < files.size(); j++) {
			read(files[j], keyFileName);
		}
	}
}

void Gallery::read(const string &filename, const string &keyfilename) {
	Enemy e(keyfilename);	//throws std::runtime_error if can't load key file
	std::ifstream enemyStream(filename);
	if(enemyStream.fail()){
		throw std::runtime_error("Gallery: Could not find file: " + filename + '\n');
	}

	bool enemyRead;
	do {
		try {
			enemyRead = e.read(enemyStream);
			if (enemyRead) {
				add(e);
			}
		} catch (std::runtime_error& err) {
			enemyRead = true;
		}
	} while (enemyRead);
	enemyStream.close();
}

void Gallery::add(const Enemy& enemy) {
	Enemy localEnemy(enemy);
	localEnemy.setGallery(this);
	enemies.push_back(localEnemy);
}

void Gallery::clear() {
	enemies.clear();
}

size_t Gallery::size() const {
	return enemies.size();
}

bool Gallery::empty() const {
	return enemies.empty();
}

const Enemy* Gallery::get(size_t n) const {
	return &enemies[n];
}

Enemy* Gallery::get(size_t n) {
	return &enemies[n];
}


std::ostream& operator<<( std::ostream &os, const Gallery &gallery ) {
	for (size_t i = 0; i < gallery.size(); i++) {
		gallery.get(i)->write(os);
		os << '\n';
	}
	return os;
}
