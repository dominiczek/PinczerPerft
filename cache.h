#ifndef CACHE_H_
#define CACHE_H_

#include <memory>

namespace{

	template <typename type>
	class Cache {
		std::unique_ptr<U64[]> keys;
		std::unique_ptr<type[]> values;
		unsigned int size;
		bool isReady = false;

	public:
		inline void Initialize(unsigned int s) {
			if(isReady) {
				return;
			}
			size = s;
			keys = std::make_unique<U64[]>(s);
			values = std::make_unique<type[]>(s);

			for(unsigned i=0;i<size;i++) {
				keys[i] = 0;
				values[i] = 0;
			}
			isReady = true;
		}

		inline void put(U64 key, type value) {
			unsigned int index = key % size;
			keys[index] = key;
			values[index] = value;
		}

		inline type get(U64 key) {
			unsigned int index = key % size;

			if(keys[index] == key) {
				return values[index];
			} else {
				return 0;
			}
		}

		inline void CleanUp() {
			keys.release();
			values.release();
			isReady = false;
		}
	};

	Cache<unsigned int> cache;
}


namespace CACHE {
	inline void Initialize(unsigned int s) {
		cache.Initialize(s);
	}

	inline void put(U64 key, U64 value) {
		cache.put(key, value);
	}

	inline U64 get(U64 key) {
		return cache.get(key);
	}

	inline void CleanUp() {
		cache.CleanUp();
	}
}

#endif
