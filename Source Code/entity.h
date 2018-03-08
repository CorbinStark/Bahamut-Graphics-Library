///////////////////////////////////////////////////////////////////////////
// FILE:                       entity.h                                  //
///////////////////////////////////////////////////////////////////////////
//                      BAHAMUT GRAPHICS LIBRARY                         //
//                        Author: Corbin Stark                           //
///////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Corbin Stark                                       //
//                                                                       //
// Permission is hereby granted, free of charge, to any person obtaining //
// a copy of this software and associated documentation files (the       //
// "Software"), to deal in the Software without restriction, including   //
// without limitation the rights to use, copy, modify, merge, publish,   //
// distribute, sublicense, and/or sell copies of the Software, and to    //
// permit persons to whom the Software is furnished to do so, subject to //
// the following conditions:                                             //
//                                                                       //
// The above copyright notice and this permission notice shall be        //
// included in all copies or substantial portions of the Software.       //
//                                                                       //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       //
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    //
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.//
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  //
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  //
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     //
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                //
///////////////////////////////////////////////////////////////////////////

#ifndef ENTITY_H
#define ENTITY_H

#include <unordered_map>
#include <memory>
#include "component.h"
#include "defines.h"

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

#endif