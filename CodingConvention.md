## 编码约定
1. 传参（持有的，传shared_ptr<T>，非持有的，传 T&）
2. 返回值（不再持有的，传T或unique_ptr<T>，继续持有的，传shared_ptr<T>）
3. 属性Get T/const T& PropertyName()
4. 属性Set void SetPropertyName(const T&)
