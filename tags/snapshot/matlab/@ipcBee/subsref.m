function [ output_args output_args2 output_args3] = subsref( self, args )
%SUBSREF Summary of this function goes here
%   Detailed explanation goes here

if strcmp(args(1).type, '()') 
%     output_args = peek(self, args.subs{1});
% skip
elseif strcmp(args(1).type, '.')
    
    if  strcmp(args(1).subs, 'grab_color')
        output_args = grab_color( self );
        
     elseif  strcmp(args(1).subs, 'grab')
        [output_args output_args2] = grab_color_and_depth( self );
        
     elseif  strcmp(args(1).subs, 'grab_all')
        [output_args output_args2 output_args3] = grab_all( self ); 
               
    elseif  strcmp(args(1).subs, 'open')
        open(self);
        
    elseif  strcmp(args(1).subs, 'close')
        close(self);        
        clear mexIpcBee
    end
        
end
    