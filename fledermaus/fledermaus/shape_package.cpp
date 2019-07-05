#include "stdafx.h"
#include "shape_package.h"

void maus::ShapeDirectoryFolder::add_items_from_sad(const sad::Node & sad_tree)
{
	for (auto iter = sad_tree.scbegin(); iter != sad_tree.cend(); ++iter) {
		auto jter = iter->scbegin();
		std::string child_type{ jter->get_value() };
		++jter;
		if (child_type == "leaf") {
			std::string container_name{ jter->get_value() }; // leaf container
			++jter;
			child_items.push_back(std::unique_ptr<ShapeDirectory>(new ShapeDirectoryLeaf(container_name, *jter)));
		}
		else if (child_type == "directory") {
			std::string container_name{ jter->get_value() }; // directory container
			++jter;
			child_items.push_back(std::unique_ptr<ShapeDirectory>(new ShapeDirectoryFolder(container_name, *jter)));
		}
		else {
			throw 1; // ##
		}
		
	}
}

maus::ShapeDirectoryLeaf::ShapeDirectoryLeaf(const std::string & container_name, const sad::Node & sad_tree) : ShapeDirectory(container_name)
{
	for (auto iter = sad_tree.scbegin(); iter != sad_tree.cend(); ++iter) {
		shapes.emplace_back(new PathShape(*iter));
	}
}
