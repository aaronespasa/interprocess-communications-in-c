struct tuple {
    string value1<256>;
    int value2;
    double value3;
};

typedef int status;

program TUPLE_SERVICE {
    version TUPLE_SERVICE_V1 {
        status init_service() = 1;
        status set_tuple(int key, string value1, int value2, double value3) = 2;
        tuple get_tuple(int key) = 3;
        status modify_tuple(int key, string value1, int value2, double value3) = 4;
        status delete_tuple(int key) = 5;
        status check_existence(int key) = 6;
        status copy_tuple(int key1, int key2) = 7;
    } = 1;
} = 13391170;