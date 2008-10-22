/*
 *  testcore.cpp
 *  vf_rbpf_slam2d
 *
 *  Created by andera on 22/09/08.
 *  Copyright 2008 Sapienza, Università di Roma. All rights reserved.
 *
 */

#include "core/metric2d_t.h"
#include "core/oriented_point2d_t.h"


#include <iostream>

int main()

{
	using namespace uor;
	
	uor::point2d  point(mm, 100, 12);
	
	std::cout << std::endl;
	std::cout << point.x(mt) << " mt" << std::endl;
	std::cout << point.y(mt) << " mt" << std::endl;
	
	point.x().m(12.3);
	point.y().mm(1000);
	
	std::cout << std::endl;
	std::cout << point.x().cm() << " cm" << std::endl;
	std::cout << point.y().cm() << " cm" << std::endl;
	
	point.x().m(0.2);
	point.y().mm(300.22);
	
	std::cout << std::endl;
	std::cout << point.x().mm() << " mm"<< std::endl;
	std::cout << point.y().mm() << " mm"<< std::endl;
	
	point2d pointA, pointB, pointC, pointD;
	
	pointA.x().m(10);
	pointA.y().cm(10);
	
	pointB.x().m(1);
	pointB.y().cm(200);
	
	pointC = pointA + pointB;
	pointD = pointA - pointB;	
	
	std::cout << std::endl;
	std::cout << pointC.x().m() << " mt"<< std::endl;
	std::cout << pointC.y().m() << " mt"<< std::endl;
	
	std::cout << std::endl;
	std::cout << pointD.x().m() << " mt"<< std::endl;
	std::cout << pointD.y().m() << " mt"<< std::endl;
	
	pointD.xy(mt, 12.2, 5.54);
	
	std::cout << std::endl;
	std::cout << pointD.x().cm() << " cm"<< std::endl;
	std::cout << pointD.y().cm() << " cm"<< std::endl;
	
	pointD.xy(cm, 200.2, 5332.54);
	std::cout << std::endl;
	std::cout << pointD.x().mm() << " mm"<< std::endl;
	std::cout << pointD.y().mm() << " mm"<< std::endl;	
	
	std::cout << pointD << std::endl;
	
	metric2d_t<> pointE(mt, 1,1);
	metric2d_t<> pointO(mt, 0, 0);
	
	std::cout << pointE << " euclidian Distance, from Origin: " << euclidian_dist(pointE, pointO) << std::endl; 
	
	//angle
	angle_t<double> angle;
	angle_t<double> angle2(rad, 0.2);
	angle_t<double> angle3(deg, 90);
	angle_t<double> angle4(rad, M_PI);
	angle_t<double> angle5(rad, -M_PI);
	
	//
	angle.deg(45);
	angle3.deg(45);
	
	//
	angle3 += angle;
	
	///
	std::cout << std::endl;
	std::cout << angle.rad() << " radians"<< std::endl;
	std::cout << angle.deg() << " degrees"<< std::endl;
	
	std::cout << std::endl;
	std::cout << angle2.rad() << " radians"<< std::endl;
	std::cout << angle2.deg() << " degrees"<< std::endl;
	
	std::cout << std::endl;
	std::cout << angle3.rad() << " radians"<< std::endl;
	std::cout << angle3.deg() << " degrees"<< std::endl;
	
	std::cout << std::endl;
	std::cout << angle4.rad() << " radians"<< std::endl;
	std::cout << angle4.deg() << " degrees"<< std::endl;
	
	std::cout << std::endl;
	std::cout << angle5.rad() << " radians"<< std::endl;
	std::cout << angle5.deg() << " degrees"<< std::endl;
	
	
	//trigonometrice
	angle_t<double> angle0;
	angle0.rad(M_PI);
	
	
	std::cout << std::endl;
	std::cout << angle0.deg() << " degrees"<< std::endl;
	std::cout << "sine " << angle0.sin() << std::endl;
	std::cout << "sine " << angle0.cos() << std::endl;
	std::cout << "tan " << angle0.tan() << std::endl;
	
	angle0.deg(45);
	
	std::cout << std::endl;
	std::cout << angle0.deg() << " degrees"<< std::endl;
	std::cout << "sine " << angle0.sin() << std::endl;
	std::cout << "sine " << angle0.cos() << std::endl;
	std::cout << "tan " << angle0.tan() << std::endl;
	
	angle0.deg(90);
	
	try {
		std::cout << std::endl;
		std::cout << angle0.deg() << " degrees"<< std::endl;
		std::cout << "sine " << angle0.sin() << std::endl;
		std::cout << "sine " << angle0.cos() << std::endl;
		std::cout << "tan " << angle0.tan() << std::endl;
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}
	
	angle3.rad(M_PI);
	angle2.rad(M_PI_2);
	
	angle0 = angle3 + angle2;
	
	std::cout << angle0;
	
	angle0 = angle3 - angle2;
	
	std::cout << angle0;
	
	try {
		angle_t<double> anglexy(0,0);
		std::cout << anglexy;
		
	}
	catch (std::runtime_error& e) {
		std::cout << e.what() << std::endl;
	}

	angle_t<double> anglexy2(1,1);
	std::cout << anglexy2 << std::endl;
	
	
	oriented_point2d_t<double, double> pose0;
	std::cout << pose0 << std::endl;
	
	pointD.xy(mt,1.23, 0.232);
	angle0.deg(33);
	
	oriented_point2d_t<> pose1(pointD, angle0);
		std::cout << pose1 << std::endl;
	
	pose1.angle().deg(90);
		std::cout << pose1 << std::endl;
	
	angle_t<double> angleb = pose1.angle();
	std::cout << angleb << std::endl;
	
	angle_t<double> anglec = pose1.angle();
	anglec.deg(42);
	std::cout << anglec << std::endl;
	std::cout << pose1 << std::endl;
	
	pose1.angle().deg(24);

	std::cout << anglec << std::endl;
	std::cout << pose1 << std::endl;
	
	oriented_point2d_t<> pose2(12.2, 2.22, cvt<>::deg2rad(22.1) );
		std::cout << pose2 << std::endl;
	
	return 0;
	
}

