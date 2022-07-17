## 编码约定
1. 传参（持有的，传shared_ptr<T>，非持有的，传 T&）
2. 返回值（不再持有的，传T或unique_ptr<T>，继续持有的，传shared_ptr<T>）
3. 属性Get T/const T& PropertyName()
4. 属性Set void SetPropertyName(const T&)
5. 异常抛出时机和类型，直接遵循dotnet的meta file里的注释
6. 为了保证头文件简洁，头文件引入private impl隐藏实现，被密集创建的对象除外
7. 只有一个numeric type即最大int64_t
