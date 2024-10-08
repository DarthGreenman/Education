/* menubar.h */

#ifndef RACE_SIMULATOR_MENUBAR_H
#define RACE_SIMULATOR_MENUBAR_H

namespace ui
{
	/* Идентификатор строки меню основной программы. */
	enum class Barkey : char {
		QUIT = 'Q', BAR_01 = 'A', BAR_02, BAR_03, BAR_04, BAR_05, BAR_06, BAR_07
	};

	inline Barkey& operator++(Barkey& key) {
		key = static_cast<Barkey>(static_cast<char>(key) + 1);
		return key;
	}

	inline Barkey operator++(Barkey& key, int) {
		Barkey prv_key{ key };
		key = static_cast<Barkey>(static_cast<char>(key) + 1);
		return prv_key;
	}
}
#endif /* RACE_SIMULATOR_MENUBAR_H */