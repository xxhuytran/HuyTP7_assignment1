#include <iostream>
#include <stdexcept>
#include <string>

// Bai tap: ExpressOrder ke thua Order va Trackable.
class Order
{
private:
    // private: chi cac ham cua Order duoc truy cap truc tiep.
    double weightKg = 0;
    std::string customerName;

protected:
    // protected: Order va cac class con duoc goi; main() khong duoc goi.
    std::string getCustomerName() const
    {
        return customerName;
    }

public:
    // public: code ben ngoai va class con deu duoc goi.
    Order(const std::string &name, double weight) : customerName(name)
    {
        if (!setWeight(weight))
        {
            throw std::invalid_argument("Khoi luong khong hop le");
        }
    }

    virtual ~Order() = default;

    // Order chi quy dinh: moi loai don phai tinh duoc phi.
    // = 0 nghia la Order khong co cong thuc chung, nen khong tao truc tiep Order.
    virtual double calcFee() const = 0;

    bool setWeight(double weight)
    {
        if (weight <= 0)
        {
            return false;
        }
        weightKg = weight;
        return true;
    }

    double getWeight() const
    {
        return weightKg;
    }

    void printInfo() const
    {
        std::cout << customerName << " - " << getWeight() << " kg\n";
    }
};

class StandardOrder : public Order
{
public:
    StandardOrder(const std::string &name, double weight) : Order(name, weight) {}

    double calcFee() const override
    {
        return getWeight() * 5000;
    }

};

class Trackable
{
private:
    std::string mTrackingCode;

public:
    Trackable(const std::string &trackingCode) : mTrackingCode(trackingCode)
    {
        if (trackingCode.empty())
        {
            throw std::invalid_argument("Ma theo doi khong duoc rong");
        }
    }

    std::string getTrackingCode() const
    {
        return mTrackingCode;
    }
};

class ExpressOrder : public Order, public Trackable
{
private:
    double mPriorityFee = 0;

public:
    ExpressOrder(const std::string &name, double weight, double priorityFee,
                 const std::string &trackingCode)
        : Order(name, weight), Trackable(trackingCode), mPriorityFee(priorityFee)
    {
        if (priorityFee < 0)
        {
            throw std::invalid_argument("Phu phi khong hop le");
        }
    }

    double calcFee() const override
    {
        return getWeight() * 10000 + mPriorityFee;
    }

    double getPriorityFee() const
    {
        return mPriorityFee;
    }

    void printDetails() const
    {
        std::cout << getCustomerName() << " - " << getWeight() << " kg\n";
        std::cout << "Ma theo doi: " << getTrackingCode() << "\n";
        std::cout << "Phu phi: " << getPriorityFee() << " VND\n";
        std::cout << "Tong phi: " << calcFee() << " VND\n";
    }
};

// Vi du 1: moi nhanh co mot phan Order rieng.
class TrackedOrder : public Order
{
public:
    TrackedOrder(const std::string &name, double weight) : Order(name, weight) {}
};

class InsuredOrder : public Order
{
public:
    InsuredOrder(const std::string &name, double weight) : Order(name, weight) {}
};

class InternationalOrder : public TrackedOrder, public InsuredOrder
{
public:
    InternationalOrder(const std::string &name, double weight)
        : TrackedOrder(name, weight), InsuredOrder(name, weight) {}

    double calcFee() const override
    {
        // Phai chi ro nhanh, vi co hai phan Order.
        return TrackedOrder::getWeight() * 10000;
    }
};

// Vi du 2: virtual inheritance cho hai nhanh dung chung mot phan Order.
class VirtualTrackedOrder : virtual public Order
{
public:
    VirtualTrackedOrder(const std::string &name, double weight) : Order(name, weight) {}
};

class VirtualInsuredOrder : virtual public Order
{
public:
    VirtualInsuredOrder(const std::string &name, double weight) : Order(name, weight) {}
};

class VirtualInternationalOrder : public VirtualTrackedOrder, public VirtualInsuredOrder
{
public:
    VirtualInternationalOrder(const std::string &name, double weight)
        : Order(name, weight), VirtualTrackedOrder(name, weight), VirtualInsuredOrder(name, weight) {}

    double calcFee() const override
    {
        return getWeight() * 10000;
    }
};

// Ham nay nam NGOAI cac class va chi biet kieu chung Order.
// const Order& tham chieu den object that, khong sao chep va khong sua object.
void showOrderFee(const Order& order)
{
    // getWeight() va calcFee() la public cua Order.
    // Khong can biet don nay la StandardOrder hay ExpressOrder.
    std::cout << "Khoi luong: " << order.getWeight() << " kg, phi: "
              << order.calcFee() << " VND\n";
    // calcFee() la virtual: C++ chay cong thuc cua class con tuong ung.
}

int main()
{
    // Order firstOrder("An", 10);
    // Order secondOrder("Binh", 5.5);

    // firstOrder.printInfo();
    // secondOrder.printInfo();

    // try
    // {
    //     Order invalidOrder("Loi", -10);
    //     invalidOrder.printInfo();
    // }
    // catch (const std::invalid_argument &error)
    // {
    //     std::cout << error.what() << '\n';
    // }

    ExpressOrder expressOrder("express order", 10, 20000, "EXP001");
    StandardOrder standardOrder("standard order", 10);

    // Cung mot ham nhan Order&, nhung nhan duoc hai loai don khac nhau.
    showOrderFee(standardOrder); // 10 * 5000 = 50000 VND
    showOrderFee(expressOrder);  // 10 * 10000 + 20000 = 120000 VND

    std::cout << "expressOrder fee " << expressOrder.calcFee() << "\n";
    std::cout << "standardOrder fee " << standardOrder.calcFee() << "\n";
    expressOrder.printDetails();

    // std::cout << expressOrder.weightKg;          // private cua Order
    // std::cout << expressOrder.getCustomerName(); // protected cua Order
    // std::cout << expressOrder.mPriorityFee;      // private cua ExpressOrder

    try
    {
        ExpressOrder invalidExpressOrder("Loi", 10, -1, "EXP002");
        invalidExpressOrder.printDetails();
    }
    catch (const std::invalid_argument &error)
    {
        std::cout << error.what() << '\n';
    }

    try
    {
        ExpressOrder noTrackingCode("Loi", 10, 20000, "");
        noTrackingCode.printDetails();
    }
    catch (const std::invalid_argument &error)
    {
        std::cout << error.what() << '\n';
    }

    // đa hình thông qua tham chiếu
    Order& firstOrder = standardOrder;
    Order& secondOrder = expressOrder;
    std::cout << "Phi don thuong: " << firstOrder.calcFee() << '\n';
    std::cout << "Phi don hoa toc: " << secondOrder.calcFee() << '\n';


    // đa hình thông qua con trỏ
    Order* order = &standardOrder;
    std::cout << order->calcFee() << '\n';

    Order* newOrder = new StandardOrder("Don moi", 10);
    std::cout << newOrder->calcFee() << '\n';
    delete newOrder; // delete object class con qua con trỏ kiểu cha. gọi theo thứ tự ~StandardOrder() → ~Order()

    InternationalOrder interOrder("Diamond", 10);
    // interOrder.setWeight(12); // "InternationalOrder::setWeight" is ambiguousC/C++(266)

    VirtualInternationalOrder fixedDiamond("Fixed diamond", 10);
    fixedDiamond.setWeight(12); // khi setWeight như thế này, nó sẽ tự động gọi về class cha của nó (Order) để thực thi setWeight
    return 0;
}
