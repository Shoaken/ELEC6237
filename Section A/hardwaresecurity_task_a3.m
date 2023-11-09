%define set of colours for visualization
colors = ['r','b','k','g'];
%precomputed hamming weights (number of ones) for all 0:65535 values
HWTab = sum(dec2bin(0:65535).' == '1');
%The Key
key = hex2dec('6CE3');
%Number of measurements for each combination of IN
N = 10;
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
%Matrix to store the expected execution times for the combinations of all the possible values of IN and keys:
timeModel = zeros(16,17);
%the first column corresponds to the average execution times obtained
%after the N measurements:
timeModel(:,1) = avgExcTime;
for iCtrkey = 0:15
        for iCtrlIN = 0:15
        %The expected execution time is the expected number of ones in the result of the XOR between the input nibble and the guessed key, plus the expected number of ones in the random part (12/2)
        timeModel(iCtrkey+1,iCtrlIN+2) =HWTab(bitxor(uint8(iCtrlIN),uint8(iCtrkey)) + 1) + 6;
        end
end
timeModel

% Rm = corrcoef(timeModel);
% Rc = Rm(1,2:17);
% [corr,idx0] = max(Rc);
% guessedKeyNibble = idx0-1;
% guessedKeyNibble

Testmatrix1 =corr(timeModel,'Type', 'Pearson');
Testmatrix1;
Rp = Testmatrix1(1,2:17);
[corrPearson,idx1] = max(Rp);
guessedKeyNibble1 = idx1-1;
guessedKeyNibble1

Testmatrix2 =corr(timeModel,'Type', 'Spearman');
Testmatrix2;
Rs = Testmatrix2(1,2:17);
[corrSpearman, idx2] = max(Rs);
guessedKeyNibble2= idx2-1;
guessedKeyNibble2

Testmatrix3 =corr(timeModel,'Type', 'Kendall');
Testmatrix3;
Rk = Testmatrix3(1,2:17);
[corrKendall, idx3] = max(Rk);
guessedKeyNibble3= idx3-1;
guessedKeyNibble3

