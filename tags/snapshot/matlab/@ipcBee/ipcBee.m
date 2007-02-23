function obj = ipcBee
% The Funtion Table ID
% Same as the order in which the functions are inserted into the 
% DLL function table.
% It is quite weird but it avoids the string parsing. 
self.CREATE         = 0;
self.OPEN           = 1;
self.CLOSE          = 2;
self.GRABCOLOR      = 3;
self.GRABRGBXYZ		= 4;
self.GRABALL        = 5;
%
self.rows   = 0;
%
self.cols   = 0;

% Call the Bumblebee Gateway
self.cnx = mexIpcBee(self.CREATE); 

% 
obj = class(self, 'ipcBee');
return