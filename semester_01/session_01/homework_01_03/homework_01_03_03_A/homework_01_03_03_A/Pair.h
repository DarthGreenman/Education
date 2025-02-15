#pragma once
#ifndef PAIR_H
#define PAIR_H

template<typename K, typename N>
class Pair {
private:
	K key_;
	N value_;

public:
	Pair(const K& key, const N& value)
		: key_{ key }, value_{ value } {
	}
	Pair()
		: key_{ K() }, value_{ N() } {
	}
	~Pair() {
	}

	Pair& operator= (const Pair& rhv) {
		key_ = rhv.key_;
		value_ = rhv.value_;
		return *this;
	}

	void set_key(const K& key) {
		key_ = key;
	}
	void set_value(const N& value) {
		value_ = value;
	}

	const K& get_key() const {
		return key_;
	}
	const N& get_value() const {
		return value_;
	}
	const bool find(const K& key) const {
		return true;
	}
};
#endif /*PAIR_H*/