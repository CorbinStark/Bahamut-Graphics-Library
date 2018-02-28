#pragma once
#include <unordered_map>
#include <memory>
#include "component.h"

class Entity {
public:
	template<class T, class... TArgs>
	T* addComp(TArgs&&... mArgs) {
		if (getComp<T>() == NULL) {
			auto result(new T(std::forward<TArgs>(mArgs)...));

			_components.emplace(&typeid(T), std::unique_ptr<Component>(result));

			return result;
		}
		else {
			std::cout << "COMPONENT ALREADY EXISTS!" << std::endl;
			return getComp<T>();
		}
	}

	template<class T>
	T* getComp() {
		try {
			return static_cast<T*>(_components.at(&typeid(T)).get());
		}
		catch (std::exception er) {
			//std::cout << "Could not get component! Make sure to check if it's null before using it. \nError: " << er.what() << std::endl;
			return NULL;
		}
	}

	template<class T>
	void removeComp() {
		try {
			_components.at(&typeid(T)).release();
			_components.erase(&typeid(T));
		}
		catch (std::exception er) {
			//std::cout << "Could not remove component! \nError: " << er.what() << std::endl;
		}
	}
private:

	std::unordered_map<const std::type_info*, std::unique_ptr<Component>> _components;
};