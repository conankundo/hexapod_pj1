import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Tâm và bán kính của hình tròn
x_center = 106  # Tọa độ x của tâm
y_center = 0    # Tọa độ y của tâm
z_center = 145  # Tọa độ z của tâm
r = 40      # Bán kính của hình tròn

# Số điểm để vẽ hình tròn (tăng số lượng để có đường tròn mượt mà hơn)
num_points_cir = 40
num_points_line = 40
# Góc bắt đầu và kết thúc của nửa hình tròn (tính theo radian)
theta_start = -np.pi/2
theta_end = np.pi/2  # Nửa vòng tròn

# Tạo các góc từ góc bắt đầu đến góc kết thúc
theta = np.linspace(theta_start, theta_end, num_points_cir)

# Tính toán tọa độ của các điểm trên nửa hình tròn
z_circle = -r * np.cos(theta) + z_center
y_circle = r * np.sin(theta) + y_center
x_circle = x_center * np.ones(num_points_cir) # Tất cả điểm trên mặt phẳng z = z_center
# Đường thẳng
end_point = np.array([x_circle[0], y_circle[0], z_circle[0]])  # Tọa độ của điểm bắt đầu
start_point = np.array([x_circle[-1], y_circle[-1], z_circle[-1]])    # Tọa độ của điểm kết thúc
x_lines = np.linspace(start_point[0], end_point[0], num_points_line)
y_lines = np.linspace(start_point[1], end_point[1], num_points_line)
z_lines = np.linspace(start_point[2], end_point[2], num_points_line)
# Tổng hợp
x_points_1haft = np.round(np.concatenate((x_circle, x_lines)),1)
y_points_1haft = np.round(np.concatenate((y_circle, y_lines)),1)
z_points_1haft = np.round(np.concatenate((z_circle, z_lines)),1)
x_points_2haft = np.round(np.concatenate((x_lines, x_circle)),1)
y_points_2haft = np.round(np.concatenate((y_lines, y_circle)),1)
z_points_2haft = np.round(np.concatenate((z_lines, z_circle)),1)
x_points = np.round(np.concatenate((x_points_1haft, x_points_2haft)),1)
y_points = np.round(np.concatenate((y_points_1haft, y_points_2haft)),1)
z_points = np.round(np.concatenate((z_points_1haft, z_points_2haft)),1)
# print(x_points)
# print(y_points)
# print(z_points)
# Vẽ nửa hình tròn
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot(x_points, y_points, z_points, 'b', linewidth=2)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Nửa hình tròn trong không gian 3D')
plt.show()
print('float x[',len(x_points),'] = {', ','.join(map(str, x_points)), '};')
print('float y[',len(y_points),'] = {', ','.join(map(str, y_points)), '};')
# print("z: ", *z_points, sep="," )
print('float z[',len(z_points),'] = {', ','.join(map(str, z_points)), '};')
print(len(z_points))