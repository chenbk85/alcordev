function close( self)
%UNTITLED1 Summary of this function goes here
%   Detailed explanation goes here
mexIpcBee(self.CLOSE, self.cnx); 
clear ipc_bee_mex