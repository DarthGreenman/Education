/* Geometric.h*/

#ifndef GEOMETRIC_SHAPES_H
#define GEOMETRIC_SHAPES_H

#ifdef GEOMETRIC_SHAPES_EXPORT
#define GEOMETRIC_SHAPES_DLL __declspec(dllexport)
#else
#define GEOMETRIC_SHAPES_DLL __declspec(dllimport)
#endif

#include "Types.h"
#include <memory>

namespace geo {
	extern GEOMETRIC_SHAPES_DLL IShape* make_shape(const Param_list& shape);
}
#endif /* GEOMETRIC_SHAPES_H */