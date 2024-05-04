clc;clear;
out = read_vis('..\Examples\2BLOCKS2WAY2022_IX.VIS_T0000');
out.SATURATION.Data(:,1) = 0.5;
write_vis('..\Examples\2BLOCKS2WAY2022_IX.VIS_T0000', 'SATURATION', out.SATURATION.Data(:,1));
out = read_vis('..\Examples\2BLOCKS2WAY2022_IX.VIS_T0000');