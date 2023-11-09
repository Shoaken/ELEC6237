%the KEY
key = 13; %Key=1101
%user inputs
IN = [1 2 5 9 13];
%number of user inputs
numIN = length(IN);
excTime = zeros (numIN,1);
predTime = zeros(numIN,16);
%precomputed Number of ones for all 0:15 values
HWTab = sum(dec2bin(0:15).' == '1');
for iCtrlIN = 1:numIN
    %Execution times (number of ones of the result of the XOR between the key and the user inputs):
    excTime(iCtrlIN) = HWTab(bitxor(uint8(IN(iCtrlIN)),uint8(key))+1);
    for iCtrlKEY = 0:15
        %predicted execution times (number of ones of the result of the XOR between all the possible keys and the user inputs):
        predTime(iCtrlIN,iCtrlKEY+1) = HWTab(bitxor(uint8(IN(iCtrlIN)),uint8(iCtrlKEY))+1);
    end
end
%display execution times
excTime
%display predicted execution times
predTime
