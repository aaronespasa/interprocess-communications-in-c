struct Value {
    int key;
    string value1<256>;
    int value2;
    double value3;
};

struct TwoKeys {
    int key1;
    int key2;
};

program TUPLE_SERVICE {
    version TUPLE_SERVICE_V1 {
        int INIT() = 1;
        int SET_VALUE(Value value) = 2;
        Value GET_VALUE(int key) = 3;
        int MODIFY_VALUE(Value value) = 4;
        int DELETE_KEY(int key) = 5;
        int EXIST(int key) = 6;
        int COPY_KEY(TwoKeys keys) = 7;
    } = 1;
} = 13391170;