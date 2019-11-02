//
// Created by Dad on 11/1/19.
//

#ifndef CS253HW5_GALLERY_H
#define CS253HW5_GALLERY_H

#include <string>
#include <vector>

using std::string;
//class Enemy;
#include "Enemy.h"

class Gallery {
public:
	Gallery(const string& s1, const string& s2);
	Gallery(const string& s1, const string& s2, const string& s3);
	Gallery(const string& s1, const string& s2, const string& s3, const string& s4);
	Gallery(const string& s1, const string& s2, const string& s3, const string& s4, const string& s5);
	Gallery(const string& s1, const string& s2, const string& s3, const string& s4, const string& s5, const string& s6);
	Gallery(std::vector<std::string> files);

	Gallery(const Gallery& other) = default;
	Gallery& operator=(const Gallery& other) = default;
	virtual ~Gallery() = default;

	void read(const string& filename, const string& keyfilename);
	void add(const Enemy& enemy);
	void clear();
	size_t size() const;
	bool empty() const;
	const Enemy* get(size_t n) const;
	Enemy* get(size_t n);

private:
	std::vector<Enemy> enemies;
};

std::ostream& operator<<( std::ostream &os, const Gallery &person );

#endif //CS253HW5_GALLERY_H
