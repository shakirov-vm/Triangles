#pragma once

void handle_seg_n_trian(Triangle& segment_tr, Triangle& trian, Surface& trian_surf);
void compare_triangles(Triangle& first, Triangle& second);
void reverse_normal(Surface& surf);
bool is_point(Triangle& trian);
void handle_2_seg(Triangle& zero, Triangle& first);
void handle_2_point(Triangle& P1, Triangle& P2);
void handle_seg_n_point(Triangle& segment_tr, Triangle& point_tr);
void handle_trian_n_point(Triangle& trian, Triangle& point_tr);
bool intersect_segments(Segment const &first, Segment const &second);

