function [ rgb ] = grab_color( self )
%GRAB_COLOR Summary of this function goes here
%   Detailed explanation goes here
rgb = ipc_bee_mex(self.GRABCOLOR, self.cnx);
