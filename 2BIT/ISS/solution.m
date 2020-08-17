%nacteni obrazku
obrazek=imread('../xkolec07.bmp');

%zaostreni obrazku
h = [-0.5 -0.5 -0.5; -0.5 5 -0.5; -0.5 -0.5 -0.5];
obrazek1 = imfilter(obrazek, h);
imwrite(obrazek1, '../step1.bmp', 'bmp');

%otoceni obrazku
obrazek2 = fliplr(obrazek1);
imwrite(obrazek2, '../step2.bmp', 'bmp');

%medianovy filtr
obrazek3 = medfilt2(obrazek2, [5 5]);
imwrite(obrazek3, '../step3.bmp', 'bmp');

%rozmazani obrazu
h2 = [1 1 1 1 1; 1 3 3 3 1; 1 3 9 3 1; 1 3 3 3 1; 1 1 1 1 1] / 49;
obrazek4 = imfilter(obrazek3, h2);
imwrite(obrazek4, '../step4.bmp', 'bmp');

%chyba v obrazku
obrazek_ot = fliplr(obrazek);
obrazek_ot_d = double(obrazek_ot);
obrazek4_d = double(obrazek4);
chyba = 0;
chyba = sum(sum(double(abs(obrazek_ot_d-obrazek4_d))));
chyba = chyba/512/512;
f = fopen('../reseni.txt','w+');
fprintf(f, 'chyba=%.5f\n', chyba);

%roztazeni histogramu
max_obrazek5 = double(max(max(obrazek4)));
min_obrazek5 = double(min(min(obrazek4)));
obrazek5=imadjust(obrazek4, [min_obrazek5/255, max_obrazek5/255], [0,1]);
imwrite(obrazek5, '../step5.bmp', 'bmp');

%stredni hodnota a smerodatna odchylka
obrazek4_d=double(obrazek4);
obrazek5_d=double(obrazek5);
std_no_hist=std2(obrazek4_d);
mean_no_hist=mean(mean(obrazek4_d));
mean_hist=mean(mean(obrazek5_d));
std_hist=std2(obrazek5_d);
fprintf(f, 'mean_no_hist=%.5f\n', mean_no_hist);
fprintf(f, 'std_no_hist=%.5f\n', std_no_hist);
fprintf(f, 'mean_hist=%.5f\n', mean_hist);
fprintf(f, 'std_hist=%.5f\n', std_hist);
fclose(f);

%kvantizace obrazu
a=0;
b=255;
N=2;
obrazek6 = round(((2^N)-1)*(double(obrazek5)-a)/(b-a))*(b-a)/((2^N)-1) + a;
obrazek6 = uint8(obrazek6);
imwrite(obrazek6, '../step6.bmp', 'bmp');