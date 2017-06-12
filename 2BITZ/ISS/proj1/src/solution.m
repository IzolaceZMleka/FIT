%XPAVLI78

iptsetpref('UseIPPL', false);
I = imread('xpavli78.bmp');

%ZAOSTRENI OBRAZU
M1 = [ -0.5 -0.5 -0.5; -0.5 5 -0.5; -0.5 -0.5 -0.5];
I1 = imfilter(I, M1);
imwrite(I1, 'step1.bmp');

%OTOCENI OBRAZU
I2 = fliplr(I1);
imwrite(I2, 'step2.bmp');

%MEDIAN OBRAZU
I3 = medfilt2(I2, [5 5]);   
imwrite(I3, 'step3.bmp');

%ROZMAZANI OBRAZU
M2 = [1 1 1 1 1; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1; 1 1 1 1 1] / 49;
I4 = imfilter(I3, M2);
imwrite(I4, 'step4.bmp');

%CHYBA V OBRAZE
I4_F = fliplr(I4);

I4_D = im2double(I4_F);
I_D = im2double(I);

I_SIZE = size(I);
I_X = min(I_SIZE);
I_Y = max(I_SIZE);

chyba = 0;

for (x=1:I_X)
    for (y=1:I_Y)
        chyba = chyba + abs(I_D(x,y)-I4_D(x,y));
    end;
end;

chyba = (chyba / (I_X * I_Y))*255;

%ROZTAZENI HISTOGRAMU
l = min(min(im2double(I4)));
h = max(max(im2double(I4)));
I5 = imadjust(I4, [l h], [0 1]);
imwrite(I5, 'step5.bmp');

%SMERODATNA ODCHYLKA
mean_no_hist = mean2(im2double(I4))*255;
std_no_hist = std2(im2double(I4))*255;
mean_hist = mean2(im2double(I5))*255;
std_hist = std2(im2double(I5))*255;

%KVANTIZACE OBRAZU
N = 2;
a = 0;
b = 255;

I6 = zeros(I_X, I_Y);
I_T = double(I5);
for(k=1:I_X)
 for(l=1:I_Y)
  I6(k,l) = round(((2^N)-1)*(I_T(k, l)-a)/(b-a))*(b-a)/((2^N)-1) + a;
 end;
end;
I6=uint8(I6);
imwrite(I6, 'step6.bmp');

%TISK

fileID = fopen('reseni.txt', 'w');
fprintf(fileID, 'chyba=%f\nmean_no_hist=%f\nstd_no_hist=%f\nmean_hist=%f\nstd_hist=%f', chyba, mean_no_hist, std_no_hist, mean_hist, std_hist);
fclose(fileID);	