function [ lrgb ,rrgb, xyz] = grab_color_and_depth( self )
%GRABALL Summary of this function goes here
%   Detailed explanation goes here
[ lrgb ,rrgb, xyz] = mexIpcBee(self.GRABALL, self.cnx);
