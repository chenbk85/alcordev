function open(self, name)
%UNTITLED1 Summary of this function goes here
%   Detailed explanation goes here
if nargin == 2
mexBee(self.OPEN, self.cnx); 
else
disp('What Camera?')
end