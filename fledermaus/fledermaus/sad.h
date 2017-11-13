#pragma once

#include <string>
#include <vector>
#include <memory>
#include <list>
#include <exception>

#include <iostream> // for debugging

namespace sad {

	/**
		costum exceptions
	**/

	class InvalidSadFileException : public std::exception {
	public:
		virtual const char* what() const override {
			return "sth bad happenden.";
		}
	};

	class ThisIsNoSadValueException : public std::exception {
	public:
		virtual const char* what() const override {
			return "this is no sad Value.";
		}
	};

	class ThisIsNoSadListException : public std::exception {
	public:
		virtual const char* what() const override {
			return "this is no sad list.";
		}
	};

	class ListEndException : public std::exception {
	public:
		virtual const char* what() const override {
			return "Reached end of a sad::List but tried to access next element.";
		}
	};

	/*
		sad tree components
	*/

	class Node {
	public:
		class const_iterator {
			std::list<std::unique_ptr<Node>>::const_iterator iter;
			const_iterator(const std::list<std::unique_ptr<Node>>::const_iterator& iter) : iter(iter) {}
		public:
			const_iterator& operator++() { ++iter; return *this; }
			const_iterator operator++(int) { const_iterator copy{ *this }; ++iter; return copy; }
			bool operator!=(const const_iterator& another) { return iter != another.iter; }
			const Node& operator*() { return **iter; }
			operator const Node*() const { return &(**iter); }
			const Node* operator->() { return &(**iter); }


			friend Node;
		};

		class save_const_iterator : public const_iterator {
			//std::list<std::unique_ptr<Node>>::const_iterator iter;
			std::list<std::unique_ptr<Node>>::const_iterator end;
			save_const_iterator(const std::list<std::unique_ptr<Node>>::const_iterator& iter, const std::list<std::unique_ptr<Node>>::const_iterator& end) : const_iterator(iter), end(end) {}
		public:
			save_const_iterator& operator++() {
				if (iter == end) throw ListEndException();
				++iter; return *this;
			}
			save_const_iterator operator++(int) {
				if (iter == end) throw ListEndException();
				save_const_iterator copy{ *this }; ++iter; return copy;
			}
			bool operator!=(const const_iterator& another) {
				return iter != another.iter;
			}
			const Node& operator*() {
				if (iter == end) throw ListEndException();
				return **iter;
			}

			const Node* operator->() {
				if (iter == end) throw ListEndException();
				return &(**iter);
			}


			friend Node;

		};


			/* returns the string of this node if its an sad::Value
			   if it is no sad::Value an ThisIsNoSadValueException is thrown */
		virtual const std::string& get_value() const = 0;

			/* returns the list of Node pointers if it's an sad::List
			   if it is no sad::List an ThisIsNoSadListException is thrown */
		virtual const std::list<std::unique_ptr<Node>>& sublist() const = 0;


		virtual const Node& get_item() const = 0;
		virtual void reset_iterator() const = 0;

		std::vector<std::string>&& to_string_vector() const;

		const_iterator cbegin() const { return const_iterator(sublist().cbegin()); }
		save_const_iterator scbegin() const { return save_const_iterator(sublist().cbegin(), sublist().cend()); }
		const_iterator cend() const { return const_iterator(sublist().cend()); }
	};

	class Value : public Node {
		std::string value;
	public:
		Value(std::string&& str) : value(std::move(str)) {}

		const std::string& get_value() const override {
			return value;
		}

		const std::list<std::unique_ptr<Node>>& sublist() const override {
			throw ThisIsNoSadListException();
		}

		const Node& get_item() const override {
			throw ThisIsNoSadListException();
		}

		void reset_iterator() const override {
			throw ThisIsNoSadListException();

		}
	};

	class List : public Node {
		std::list<std::unique_ptr<Node>> items;

		std::unique_ptr<std::list<std::unique_ptr<Node>>::const_iterator> iter;

	public:
		List() {}
		List(std::istream& istream)// f�ngt nach ( an. h�rt auf nachdem es ) oder end of file stream gelesen hat.
		{
			std::string buffer{};
			char c;
			while (istream >> c) {
				if (c == '(') {
					if (!buffer.empty()) throw InvalidSadFileException();
					buffer.clear();
					items.push_back(std::unique_ptr<Node>(new List(istream)));
					if (istream >> c) {
						if (c != ':') {
							if (c == ')') return;
							else throw InvalidSadFileException();
						}
					}
					continue;
				}
				else if (c == ')') {
					if ((buffer.empty()) && (items.size() == 0)) {
						// empty list // if you reach ) but you've never listed any string value or subcomponent and buffer is empty,
						// then there is no empty string put in the list, the list is left empty
						// <<<<< we might need an escape for lists with one emtystring element.
						return;
					}
					items.push_back(std::unique_ptr<Node>(new Value(std::move(buffer)))); // either push empty string
					return;
				}
				else if (c == ':') {
					items.push_back(std::unique_ptr<Node>(new Value(std::move(buffer))));
					buffer.clear();
				}
				else buffer.push_back(c);
			}
		}

		const std::string& get_value() const override {
			throw ThisIsNoSadValueException();
		}

		const std::list<std::unique_ptr<Node>>& sublist() const override { return items;  }

		const Node& get_item() const override {
			if (*iter == items.cend()) {
				// throw exception
			}
			return **((*iter)++);
		}
		void reset_iterator() const override { *iter = items.cbegin(); }
	};

	class File {
		std::unique_ptr<Node> tree;
	public:
		File() : tree(nullptr) {}
		File(std::istream& stream) {
			//char c; stream >> c; // avoid first (
			tree = std::unique_ptr<Node>(new List(stream));
		}
		void open(std::istream& stream) {
			tree = std::unique_ptr<Node>(new List(stream));
		}

		const Node& get_node() { return *tree;  }
	};


	std::string&& read_until_colon(std::istream& istream);

	std::vector<std::string>&& read_string_vector_component(std::istream& istream);

}


/*


class SADToken {

public:


virtual bool operator == (const SADToken& another) const = 0;

};

class SADSeparator : public SADToken { // stands for the colon.
public:
static const SADSeparator* get_p�nstance() {
static SADSeparator instance{};
return &instance;
}
public:
bool operator == (const SADToken& another) const override {
return this == get_p�nstance();
}
};

class SADOpenBracket : public SADToken {
public:
static const SADOpenBracket* get_pInstance() {
static SADOpenBracket instance{};
return &instance;
}
public:
bool operator == (const SADToken& another) const override {
return this == get_pInstance();
}
};

class SADCloseBracket : public SADToken {
static constexpr int ID{ 3 };
public:
static const SADCloseBracket* get_pInstance() {
static SADCloseBracket instance{};
return &instance;
}
public:
bool operator == (const SADToken& another) const override {
return this == get_pInstance();
}
};

class Value : public SADToken {
std::string value;
public:
Value(std::string&& another) : value(std::move(another)) {}

bool operator == (const SADToken& another) const override {
return false;
}
};

class File {
std::list<std::unique_ptr<SADToken>> items;
public:
File() : items(0) {}
File(std::istream& stream) {
std::string buffer{};
while (!stream.eof()) {
char c;
stream >> c;
if (c == '(') {
if (!buffer.empty()) {
// error heree
}
items.push_back(std::unique_ptr<SADToken>(new SADOpenBracket{}));
buffer.clear();
continue;
}
if (c == ')') {
if (!buffer.empty()) {
items.push_back(std::unique_ptr<SADToken>(new Value(std::move(buffer)));
}
items.push_back(std::unique_ptr<SADToken>(new SADCloseBracket{}));
buffer.clear();
}
if (c == ':') {
if (!)
}
buffer.push_back(c);


}
stream >> c;

}


};



*/
