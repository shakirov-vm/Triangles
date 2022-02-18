#pragma once

#include "../Geometry/Geometry3D.h"

namespace Triangles {

void handle_2_point(Triangle& P1, Triangle& P2);
void handle_seg_n_point(Triangle& segment_tr, Triangle& point_tr);
void handle_trian_n_point(Triangle& trian, Triangle& point_tr);
void handle_2_seg(Triangle& zero, Triangle& first);
void handle_seg_n_trian(Triangle& segment_tr, Triangle& trian, Surface const &trian_surf);

}