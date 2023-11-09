%define set of colours for visualization
colors = ['r','b','k','g'];
%precomputed hamming weights (number of ones) for all 0:65535 values
HWTab = sum(dec2bin(0:65535).' == '1');
%The Key
key = hex2dec('6CEF');
%Number of measurements for each combination of IN
N =246;
acumExcTime = zeros(16,N);
figure;
hold on;
%iterate over all the combinations of the first nibble of input IN
for iCtrlIN = 0:15
    %iterate over N measurements
    for iCnt = 1:(N-1)
    %generate random input IN of 16 bits:
    randomInput = round(rand*(2^16-1));
    %create a mask to remove the first nibble from the random input
    mask = bitcmp(15,'uint16');
    %apply the mask to random input:
    maskedInput = bitand(randomInput,mask);
    %replace empty nibble with the controlled part of the input(iCtrlIN):
    input = bitor(uint16(maskedInput),uint16(iCtrlIN));
    %Simulate execution of the algorithm
    MES = bitxor(uint16(input),uint16(key));
    %obtain execution time (number of bits of MES)
    excTime = HWTab(MES + 1);
    %Accumulate the execution time of all the N times that the algorithm
    %is executed for the same value of IN):
    acumExcTime(iCtrlIN+1,iCnt+1) = acumExcTime(iCtrlIN+1,iCnt)+ excTime;
    end
%Plot the progress of the average execution time at each execution of
%the algorithm for each combination of IN using different colours:
plot(1:N,acumExcTime(iCtrlIN+1,:) ./ (1:N),colors(mod(iCtrlIN,4)+1));
end
hold off;
xlabel('N of executions','FontSize',14);
ylabel('Average number of bits in MES (Average time)','FontSize',14);
set(gca,'FontSize',14);
%Display the average execution time obtained after the N measurements:
avgExcTime = acumExcTime(1:16,N) ./ N