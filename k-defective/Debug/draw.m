A = load('draw.raw');

subplot(1, 2, 1);
hold on;
plot(A(1, :));
plot(A(2, :));
plot(A(3, :));

xlabel('k (10��)');
ylabel('����ʱ�� (ms)');

legend('Base', 'Simple', 'RDS');

subplot(1, 2, 2);
hold on;
plot(A(4, :));
plot(A(5, :));
plot(A(6, :));

xlabel('k (10��)');
ylabel('�������ڵ��� (��)');

legend('Base', 'Simple', 'RDS');