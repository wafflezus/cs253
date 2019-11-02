//
// Created by Evan on 10/19/2019.
//
#ifndef ENEMY_INCLUDED
#define ENEMY_INCLUDED

#include <vector>
class Gallery;

class Enemy {
private:
    std::vector<std::string> keys, values, allowedkeys;
    bool nameflag = true, otherflag = true, linkflag = true;
	Gallery* gallery;
public:
    Enemy(const Enemy& other) = default;
	Enemy(std::string keyfile);
    ~Enemy() = default;
    Enemy& operator=(const Enemy& other) = default;
    bool read(std::istream &in);
    void write(std::ostream &out) const;
    void write(std::string filename) const;
    std::string field(std::string key) const;
    void show_name(bool b);
    void show_other(bool b);
    void show_link(bool b);
    void clear();
    size_t size() const;
    bool empty() const;

    Enemy* link(std::string relation) const;
    void setGallery(Gallery* gallery);
};
std::ostream &operator<<(std::ostream &out, Enemy e);

#endif