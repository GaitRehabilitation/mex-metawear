%% Connect to sensor
metawear('disconnect_all');
sensor1 = metawear('connect','CA:8A:CF:D8:BC:7C');
sensor2 = metawear('connect','F2:C0:45:7D:3D:43');
sensor3 = metawear('connect','F9:60:2C:3E:8B:13');
metawear('query_handlers',sensor1);
metawear('query_handlers',sensor2);
metawear('query_handlers',sensor3);

%%
metawear('teardown',sensor1);
metawear('teardown',sensor2);
metawear('teardown',sensor3);

metawear('clear_logging',sensor1);
metawear('clear_logging',sensor2);
metawear('clear_logging',sensor3);

metawear('configure_accelerometer_sample',sensor1,25);
metawear('configure_accelerometer_range',sensor1,4);
metawear('configure_accelerometer_config',sensor1);
metawear('enable_accelerometer',sensor1);

metawear('configure_gyro_sample',sensor1,25);
metawear('configure_gyro_config',sensor1);
metawear('enable_gyro',sensor1);

metawear('configure_accelerometer_sample',sensor2,25);
metawear('configure_accelerometer_range',sensor2,4);
metawear('configure_accelerometer_config',sensor2);
metawear('enable_accelerometer',sensor2);

metawear('configure_gyro_sample',sensor2,25);
metawear('configure_gyro_config',sensor2);
metawear('enable_gyro',sensor2);

metawear('configure_accelerometer_sample',sensor3,25);
metawear('configure_accelerometer_range',sensor3,4);
metawear('configure_accelerometer_config',sensor3);
metawear('enable_accelerometer',sensor3);

metawear('configure_gyro_sample',sensor3,25);
metawear('configure_gyro_config',sensor3);
metawear('enable_gyro',sensor3);

signal_acc3 = metawear('subscribe_acc',sensor3,'LOG');
signal_acc2 = metawear('subscribe_acc',sensor2,'LOG');
signal_acc1 = metawear('subscribe_acc',sensor1,'LOG');

signal_gyro3 = metawear('subscribe_gyro',sensor3,'LOG');
signal_gyro2 = metawear('subscribe_gyro',sensor2,'LOG');
signal_gyro1 = metawear('subscribe_gyro',sensor1,'LOG');


%%
metawear('start_logging',sensor1);
metawear('start_logging',sensor2);
metawear('start_logging',sensor3);
for n = 1:10
fprintf('count: %d \n',n);
pause(1);
end
%%

metawear('disconnect_all');
sensor1 = metawear('connect','CA:8A:CF:D8:BC:7C');
sensor2 = metawear('connect','F2:C0:45:7D:3D:43');
sensor3 = metawear('connect','F9:60:2C:3E:8B:13');
metawear('query_handlers',sensor1);
metawear('query_handlers',sensor2);
metawear('query_handlers',sensor3);

%%
metawear('stop_logging',sensor1);
metawear('stop_logging',sensor2);
metawear('stop_logging',sensor3);

metawear('set_connection_parameters',sensor1,7.5,7.5, 0,6000);
metawear('set_connection_parameters',sensor2,7.5,7.5, 0,6000);
metawear('set_connection_parameters',sensor3,7.5,7.5, 0,6000);

metawear('download_logging',sensor1);
metawear('download_logging',sensor2);
metawear('download_logging',sensor3);

%%
[epoch_a_1,x_a_1,y_a_1,z_a_1] = metawear('query',sensor1,signal_acc1);
[epoch_a_2,x_a_2,y_a_2,z_a_2] = metawear('query',sensor2,signal_acc2);
[epoch_a_3,x_a_3,y_a_3,z_a_3] = metawear('query',sensor3,signal_acc3);

[epoch_g_1,x_g_1,y_g_1,z_g_1] = metawear('query',sensor1,signal_gyro1);
[epoch_g_2,x_g_2,y_g_2,z_g_2] = metawear('query',sensor2,signal_gyro2);
[epoch_g_3,x_g_3,y_g_3,z_g_3] = metawear('query',sensor3,signal_gyro3);
