A = load('draw.raw');

subplot(1, 2, 1);
hold on;
plot(A(1, :));
plot(A(2, :));
plot(A(3, :));

xlabel('k (10个)');
ylabel('运行时间 (ms)');

legend('Base', 'Simple', 'RDS');

subplot(1, 2, 2);
hold on;
plot(A(4, :));
plot(A(5, :));
plot(A(6, :));

xlabel('k (10个)');
ylabel('搜索树节点数 (个)');

legend('Base', 'Simple', 'RDS');