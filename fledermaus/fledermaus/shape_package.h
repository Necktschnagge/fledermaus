#pragma once

#include <vector>
#include <memory>

#include "shapes.h"
#include "sad.h"

namespace maus {

	class ShapeDirectory {
		std::string container_name;
	public:
		ShapeDirectory(std::string conatiner_name) : container_name(conatiner_name) {}

		virtual bool is_directory() const = 0;
		bool is_leaf() const { return !is_directory(); }

		// return a const reference to vector of ptr to child structure items of a folder.
		// if you try this for a leaf, you'll get a reference to an empty vector.
		virtual const std::vector<std::unique_ptr<ShapeDirectory>>& get_child_items() const = 0;

		// return a const reference to vector of ptr to shapes of a leaf.
		// if you try this for a folder, you'll get a reference to an empty vector.
		virtual const std::vector<std::unique_ptr<Shape>>& get_child_shapes() const = 0;
	};

	class ShapeDirectoryFolder : public ShapeDirectory {
		std::vector<std::unique_ptr<ShapeDirectory>> child_items;
	public:
		ShapeDirectoryFolder(std::string container_name) : ShapeDirectory(container_name) {}
		ShapeDirectoryFolder(std::string container_name, const sad::Node& sad_tree) : ShapeDirectory(container_name) { add_items_from_sad(sad_tree); }

		bool is_directory() const override { return true; }
		void add_items_from_sad(const sad::Node& sad_tree);

		const std::vector<std::unique_ptr<ShapeDirectory>>& get_child_items() const override {
			return child_items;
		}

		const std::vector<std::unique_ptr<Shape>>& get_child_shapes() const override {
			static std::vector<std::unique_ptr<Shape>> dummy; // empty list
			return dummy;
		}

	};

	class ShapeDirectoryLeaf : public ShapeDirectory {
		std::vector<std::unique_ptr<Shape>> shapes;
	public:


		ShapeDirectoryLeaf(const std::string& container_name, const sad::Node& sad_tree);

		bool is_directory() const override { return false; }

		const std::vector<std::unique_ptr<ShapeDirectory>>& get_child_items() const override {
			static std::vector<std::unique_ptr<ShapeDirectory>> dummy;
			return dummy;
		}

		const std::vector<std::unique_ptr<Shape>>& get_child_shapes() const override {
			return shapes;
		}

	};

	class ShapePackage {
		ShapeDirectoryFolder root;

	public:
		ShapePackage() : root("root") {}

		inline ShapeDirectoryFolder& get_root_directory() { return root; };
	};

}
