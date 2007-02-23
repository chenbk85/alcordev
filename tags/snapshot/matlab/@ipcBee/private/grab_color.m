function [ rgb ] = grab_color( self )
%GRAB_COLOR Summary of this function goes here
%   Detailed explanation goes here
rgb = mexIpcBee(self.GRABCOLOR, self.cnx);
