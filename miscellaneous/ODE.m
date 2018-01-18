
N = 10000;
alpha = 0.4;
x_m = 10;
dx = x_m/N;
x = linspace(0, x_m, N);
f = zeros(1, N);
f_1 = zeros(1, N);
f(1) = 1;

for i = 1 : N - 1
    f(1, i + 1) = f(i) * (1 + alpha * dx);
end

f_1 = f(1)*exp(alpha * x);
plot(x, f_1 , 'b');
hold on
plot(x, f, 'r');
legend('Analytical', 'Numerical');