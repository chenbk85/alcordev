function obj = ptuobject
% The Funtion Table ID
% Same as the order in which the functions are inserted into the 
% DLL function table.
% It is quite weird but it avoids the string parsing. 
self.CREATE         = 0;
self.OPEN           = 1;
self.CLOSE          = 2;
self.PANTILT      	= 3;

% Call the Bumblebee Gateway
self.cnx = mexPtu(self.CREATE); 

% 
obj = class(self, 'ptuobject');
return