#include "Gallery.h"
#include <cassert>
#include <iostream>
#include <string>

using namespace std;

int main() {
	Gallery g("bond-villains", "keys", "/dev/null");
	assert(g.size() == 2);
	g.read("jaws", "keys");
	cout << g << "––––––––––––––––\n";
	assert(g.size() == 3);
	assert(!g.empty());
	g.clear();
	assert(g.size() == 0);
	assert(g.empty());

	const Gallery go("keys", "gold-odd");
	for (size_t i=0; i<go.size(); i++) {
		const Enemy *e = go.get(i);
		try {
			const Enemy *h = e->link("Henchman");   // might throw error
			cout << e->field("Name") << "’s henchman is "
				 << h->field("Name") << '\n';
		}
		catch (...) {
			cout << e->field("Name") << "’s boss is "
				 << e->link("Boss")->field("Name") << '\n';
		}
	}

	return 0;
}