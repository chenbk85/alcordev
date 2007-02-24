function [ rgb , xyz] = grab_color_and_depth( self )
%GRAB_COLOR_AND_DEPTH Summary of this function goes here
%   Detailed explanation goes here
[rgb, xyz] = mexIpcBee(self.GRABRGBXYZ, self.cnx);

