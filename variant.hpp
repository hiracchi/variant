#ifndef VARIANT_H
#define VARIANT_H

#include <cstdlib>
#include <vector>
#include <map>
#include <sstream>
#include <limits>
#include <cmath>
#include <cassert>


template <typename IteratorType, typename ValueType>
class VariantVectorIterator {
public:
    VariantVectorIterator(const IteratorType& it) : it_(it) {
    }

    VariantVectorIterator(const VariantVectorIterator& rhs) : it_(rhs.it_) {
    }

    ~VariantVectorIterator() {
    }

    VariantVectorIterator& operator=(const VariantVectorIterator& rhs) {
        if (&rhs != this) {
            this->it_ = rhs.it_;
        }
    }

public:
    ValueType& operator*() {
        return **(this->it_);
    }

    ValueType* operator->() {
        return *(this->it_);
    }

    VariantVectorIterator& operator++() {
        ++(this->it_);
        return *this;
    }

    VariantVectorIterator& operator++(int n) {
        this->it_++(n);
        return *this;
    }

    VariantVectorIterator& operator--() {
        --(this->it_);
        return *this;
    }

    VariantVectorIterator& operator--(int n) {
        this->it_--(n);
        return *this;
    }

    bool operator==(const VariantVectorIterator& rhs) const {
        return (this->it_ == rhs.it_);
    }

    bool operator!=(const VariantVectorIterator& rhs) const {
        return (this->it_ != rhs.it_);
    }

private:
    IteratorType it_;
};


template <typename IteratorType, typename ValueType>
class VariantVectorConstIterator {
public:
    VariantVectorConstIterator(const IteratorType& it) : it_(it) {
    }

    VariantVectorConstIterator(const VariantVectorConstIterator& rhs) : it_(rhs.it_) {
    }

    ~VariantVectorConstIterator() {
    }

    VariantVectorConstIterator& operator=(const VariantVectorConstIterator& rhs) {
        if (&rhs != this) {
            this->it_ = rhs.it_;
        }
    }

public:
    ValueType& operator*() const {
        return **(this->it_);
    }

    ValueType* operator->() const {
        return *(this->it_);
    }

    VariantVectorConstIterator& operator++() {
        ++(this->it_);
        return *this;
    }

    VariantVectorConstIterator& operator++(int n) {
        this->it_++(n);
        return *this;
    }

    VariantVectorConstIterator& operator--() {
        --(this->it_);
        return *this;
    }

    VariantVectorConstIterator& operator--(int n) {
        this->it_--(n);
        return *this;
    }

    bool operator==(const VariantVectorConstIterator& rhs) const {
        return (this->it_ == rhs.it_);
    }

    bool operator!=(const VariantVectorConstIterator& rhs) const {
        return (this->it_ != rhs.it_);
    }

private:
    IteratorType it_;
};


template <typename IteratorType, typename KeyType, typename ValueType>
class VariantMapIterator {
public:
    typedef std::pair<KeyType, ValueType> PairType;

public:
    VariantMapIterator(const IteratorType& it) : it_(it) {
    }

    VariantMapIterator(const VariantMapIterator& rhs) : it_(rhs.it_) {
    }

    ~VariantMapIterator() {
    }

    VariantMapIterator& operator=(const VariantMapIterator& rhs) {
        if (&rhs != this) {
            this->it_ = rhs.it_;
        }
        return *this;
    }

public:
    PairType& operator*() {
        this->pair_.first = *(this->it_->first);
        this->pair_.second = *(this->it_->second);
        return this->pair_;
    }

    PairType* operator->() {
        this->pair_.first = *(this->it_->first);
        this->pair_.second = *(this->it_->second);
        return &(this->pair_);
    }

    VariantMapIterator& operator++() {
        ++(this->it_);
        return *this;
    }

    VariantMapIterator& operator++(int n) {
        this->it_++(n);
        return *this;
    }

    VariantMapIterator& operator--() {
        --(this->it_);
        return *this;
    }

    VariantMapIterator& operator--(int n) {
        this->it_--(n);
        return *this;
    }

    bool operator==(const VariantMapIterator& rhs) const {
        return (this->it_ == rhs.it_);
    }

    bool operator!=(const VariantMapIterator& rhs) const {
        return (this->it_ != rhs.it_);
    }

private:
    IteratorType it_;
    PairType pair_;
};


template <typename IteratorType, typename KeyType, typename ValueType>
class VariantMapConstIterator {
public:
    typedef std::pair<KeyType, ValueType> PairType;

public:
    VariantMapConstIterator(const IteratorType& it) : it_(it) {
    }

    VariantMapConstIterator(const VariantMapConstIterator& rhs) : it_(rhs.it_) {
    }

    ~VariantMapConstIterator() {
    }

    VariantMapConstIterator& operator=(const VariantMapConstIterator& rhs) {
        if (&rhs != this) {
            this->it_ = rhs.it_;
        }
    }

public:
    const PairType& operator*() const {
        this->pair_.first = *(this->it_->first);
        this->pair_.second = *(this->it_->second);
        return this->pair_;
    }

    const PairType* operator->() const {
        this->pair_.first = *(this->it_->first);
        this->pair_.second = *(this->it_->second);
        return &(this->pair_);
    }

    VariantMapConstIterator& operator++() {
        ++(this->it_);
        return *this;
    }

    VariantMapConstIterator& operator++(int n) {
        this->it_++(n);
        return *this;
    }

    VariantMapConstIterator& operator--() {
        --(this->it_);
        return *this;
    }

    VariantMapConstIterator& operator--(int n) {
        this->it_--(n);
        return *this;
    }

    bool operator==(const VariantMapConstIterator& rhs) const {
        return (this->it_ == rhs.it_);
    }

    bool operator!=(const VariantMapConstIterator& rhs) const {
        return (this->it_ != rhs.it_);
    }

private:
    IteratorType it_;
    mutable PairType pair_;
};


class Variant {
protected:
    typedef std::vector<Variant*> ArrayContainerType;
    typedef std::map<Variant*, Variant*> MapContainerType;

public:
    typedef VariantVectorIterator<ArrayContainerType::iterator, Variant> ArrayIterator;
    typedef VariantVectorConstIterator<ArrayContainerType::const_iterator, Variant> ArrayConstIterator;
    typedef VariantMapIterator<MapContainerType::iterator, Variant, Variant> MapIterator;
    typedef VariantMapConstIterator<MapContainerType::const_iterator, Variant, Variant> MapConstIterator;

    enum DataType {
        BOOLEAN,
        STRING,
        INT,
        UINT,
        LONG,
        ULONG,
        DOUBLE,
        ARRAY,
        MAP,
        NONE
    };

public:
    explicit Variant(DataType dataType = NONE);
    Variant(bool value);
    Variant(char value);
    Variant(unsigned char value);
    Variant(int value);
    Variant(unsigned int value);
    Variant(long value);
    Variant(unsigned long value);
    Variant(double value);
    Variant(const char* pStr);
    Variant(const char* pStr, const std::size_t size);
    Variant(const std::string& str);
    Variant(const Variant& rhs);
    virtual ~Variant();

    Variant& operator=(const Variant& rhs);

public:
    // ====================================================================
    // properties
    // ====================================================================
    DataType type() const {
        return this->type_;
    };

    std::size_t size() const {
        std::size_t ans = 1;
        if (this->type_ == ARRAY) {
            ans = this->array_.size();
        } else if (this->type_ == MAP) {
            ans = this->map_.size();
        }
        return ans;
    };

public:
    // ====================================================================
    // vector(array) container operation
    // ====================================================================
    void resize(std::size_t size);
    void push_back(const Variant& value);
    const Variant& getAt(std::size_t index) const;
    Variant& getAt(std::size_t index);
    void setAt(std::size_t index, const Variant& value);

    ArrayIterator beginArray();
    ArrayIterator endArray();
    ArrayConstIterator beginArray() const;
    ArrayConstIterator endArray() const;

    // ====================================================================
    // map(dict) container operation
    // ====================================================================
    void add(const Variant& key, const Variant& value);
    Variant& operator[](const Variant& key);
    const Variant& operator[](const Variant& key) const;
    bool has_key(const Variant& key) const;
    void erase(const Variant& key);

    MapIterator beginMap();
    MapIterator endMap();
    MapConstIterator beginMap() const;
    MapConstIterator endMap() const;

    // ====================================================================
    // scalar operation
    // ====================================================================
    void set(const bool value);
    void set(const char value);
    void set(const unsigned char value);
    void set(const int value);
    void set(const unsigned int value);
    void set(const long value);
    void set(const unsigned long value);
    void set(const double value);
    void set(const char* pStr);
    void set(const char* pStr, const std::size_t size);
    void set(const std::string& value);

    bool get_bool() const;
    int get_int() const;
    unsigned int get_uint() const;
    long get_long() const;
    unsigned long get_ulong() const;
    double get_double() const;
    std::string get_str() const;

    bool operator==(const Variant& rhs) const;
    bool operator!=(const Variant& rhs) const {
        return !(this->operator==(rhs));
    }

    void merge(const Variant& rhs);

    /// 内容をデバッグ用の文字列として返す
    std::string str() const;

protected:
    void clearChildren();
    void copyChildren(const Variant& rhs);

    MapContainerType::iterator find(const Variant& rhs);
    MapContainerType::const_iterator find(const Variant& rhs) const;

    static const Variant& getNullObject();

protected:
    template <typename T>
    std::string x2s(const T& t) const {
        std::stringstream s;
        s << t;
        return s.str();
    }

    std::string to_upper(const std::string& str) const {
        std::ostringstream out;
        std::istringstream in(str);
        char c;

        while (in.get(c)) {
            char tc = std::toupper(c);
            out << tc;
        }

        return out.str();
    }


protected:
    union Scalar {
        Scalar(int v) {
            this->int_ = v;
        }

        Scalar(unsigned int v) {
            this->uint_ = v;
        }

        Scalar(long v) {
            this->long_ = v;
        }

        Scalar(unsigned long v) {
            this->ulong_ = v;
        }

        Scalar(double v) {
            this->double_ = v;
        }

        int int_;
        unsigned int uint_;
        long long_;
        unsigned ulong_;
        double double_;
    };

    DataType type_;
    Scalar scalar_;
    std::string str_;
    std::vector<Variant*> array_;
    std::map<Variant*, Variant*> map_;

    static Variant* pNullObject_;
};

// implementation *********************************************************
// ========================================================================
// construct / destruct
// ========================================================================
Variant* Variant::pNullObject_ = NULL;

Variant::Variant(DataType dataType) : type_(dataType), scalar_(0), str_("") {
}

Variant::Variant(const bool value) : type_(NONE), scalar_(0), str_("") {
    this->set(value);
}

Variant::Variant(const char value) : type_(NONE), scalar_(0), str_("") {
    this->set(value);
}

Variant::Variant(const unsigned char value) : type_(NONE), scalar_(0), str_("") {
    this->set(value);
}

Variant::Variant(const int value) : type_(NONE), scalar_(0), str_("") {
    this->set(value);
}

Variant::Variant(const unsigned int value) : type_(NONE), scalar_(0), str_("") {
    this->set(value);
}

Variant::Variant(const long value) : type_(NONE), scalar_(0), str_("") {
    this->set(value);
}

Variant::Variant(const unsigned long value) : type_(NONE), scalar_(0), str_("") {
    this->set(value);
}

Variant::Variant(const double value) : type_(NONE), scalar_(0), str_("") {
    this->set(value);
}

Variant::Variant(const char* pStr) : type_(NONE), scalar_(0), str_("") {
    this->set(std::string(pStr));
}

Variant::Variant(const char* pStr, const std::size_t size) : type_(NONE), scalar_(0), str_("") {
    this->set(std::string(pStr, size));
}

Variant::Variant(const std::string& str) : type_(NONE), scalar_(0), str_("") {
    this->set(str);
}

Variant::Variant(const Variant& rhs) : type_(rhs.type_), scalar_(rhs.scalar_), str_(rhs.str_) {
    this->copyChildren(rhs);
}

Variant& Variant::operator=(const Variant& rhs) {
    if (this != &rhs) {
        this->clearChildren();

        this->type_ = rhs.type_;
        this->scalar_ = rhs.scalar_;
        this->str_ = rhs.str_;
        this->copyChildren(rhs);
    }
    return *this;
}

Variant::~Variant() {
    this->clearChildren();
}

// ========================================================================
// vector operation
// ========================================================================
void Variant::resize(const std::size_t newSize) {
    this->type_ = ARRAY;

    const std::size_t oldSize = this->array_.size();
    if (newSize < oldSize) {
        // shrink
        for (std::size_t i = newSize; i < oldSize; ++i) {
            delete this->array_[i];
            this->array_[i] = NULL;
        }
        this->array_.resize(newSize);
    } else if (newSize > oldSize) {
        // expand
        this->array_.resize(newSize);
        for (std::size_t i = oldSize; i < newSize; ++i) {
            Variant* p = new Variant;
            this->array_[i] = p;
        }
    }
}

void Variant::push_back(const Variant& value) {
    this->type_ = ARRAY;

    Variant* pNew = new Variant(value);
    this->array_.push_back(pNew);
}

const Variant& Variant::getAt(const std::size_t index) const {
    if ((this->type_ == ARRAY) && (index < this->array_.size())) {
        return *(this->array_[index]);
    } else {
        return Variant::getNullObject();
    }
}

Variant& Variant::getAt(const std::size_t index) {
    assert(this->type_ == ARRAY);
    if ((index +1) > this->array_.size()) {
        this->resize(index +1);
    }
    return *(this->array_[index]);
}

void Variant::setAt(const std::size_t index, const Variant& value) {
    this->type_ = ARRAY;
    if ((index +1) > this->array_.size()) {
        this->resize(index +1);
    }
    *(this->array_[index]) = value;
}

Variant::ArrayIterator Variant::beginArray() {
    return ArrayIterator(this->array_.begin());
}

Variant::ArrayIterator Variant::endArray() {
    return ArrayIterator(this->array_.end());
}

Variant::ArrayConstIterator Variant::beginArray() const {
    return ArrayConstIterator(this->array_.begin());
}

Variant::ArrayConstIterator Variant::endArray() const {
    return ArrayConstIterator(this->array_.end());
}

// ========================================================================
// dict operation
// ========================================================================
void Variant::add(const Variant& key, const Variant& value) {
    this->type_ = MAP;
    Variant* pKey = new Variant(key);
    Variant* pValue = new Variant(value);
    this->map_[pKey] = pValue;
}

Variant& Variant::operator[](const Variant& key) {
    this->type_ = MAP;
    MapContainerType::iterator p = this->find(key);
    if (p != this->map_.end()) {
        return *(p->second);
    } else {
        Variant* pKey = new Variant(key);
        Variant* pValue = new Variant;
        this->map_[pKey] = pValue;
        return *(pValue);
    }
}

const Variant& Variant::operator[](const Variant& key) const {
    if (this->type() == MAP) {
        MapContainerType::const_iterator p = this->find(key);
        if (p != this->map_.end()) {
            return *(p->second);
        }
    }
    return this->getNullObject();
}

bool Variant::has_key(const Variant& key) const {
    bool answer = false;
    if (this->type() == MAP) {
        MapContainerType::const_iterator p = this->find(key);
        if (p != this->map_.end()) {
            answer = true;
        }
    }
    return answer;
}

Variant::MapContainerType::iterator Variant::find(const Variant& key) {
    MapContainerType::iterator answer = this->map_.end();
    if (this->map_.size() > 0) {
        MapContainerType::iterator pEnd = this->map_.end();
        for (MapContainerType::iterator p = this->map_.begin(); p != pEnd; ++p) {
            const Variant tmp = *(p->first);
            if (tmp == key) {
                answer = p;
                break;
            }
        }
    }

    return answer;
}

Variant::MapContainerType::const_iterator Variant::find(const Variant& key) const {
    MapContainerType::const_iterator answer = this->map_.end();

    MapContainerType::const_iterator pEnd = this->map_.end();
    for (MapContainerType::const_iterator p = this->map_.begin(); p != pEnd; ++p) {
        if (*(p->first) == key) {
            answer = p;
            break;
        }
    }
    return answer;
}

void Variant::erase(const Variant& key) {
    MapContainerType::iterator p = this->map_.begin();
    MapContainerType::iterator pEnd = this->map_.end();
    while (p != pEnd) {
        if (*(p->first) == key) {
            delete p->second;
            p->second = NULL;

            delete p->first;
            this->map_.erase(p++);
            break;
        } else {
            ++p;
        }
    }
}

Variant::MapIterator Variant::beginMap() {
    return MapIterator(this->map_.begin());
}

Variant::MapIterator Variant::endMap() {
    return MapIterator(this->map_.end());
}

Variant::MapConstIterator Variant::beginMap() const {
    return MapConstIterator(this->map_.begin());
}

Variant::MapConstIterator Variant::endMap() const
{
    return MapConstIterator(this->map_.end());
}



// ========================================================================
// scalar operation
// ========================================================================
bool Variant::get_bool() const {
    bool answer = false;
    switch (this->type()) {
    case BOOLEAN:
        answer = (this->scalar_.int_ != 0);
        break;

    case STRING:
        {
            const std::string check = this->to_upper(this->str_);
            if ((check == "TRUE") ||
                (check == "YES") ||
                (check == "ON") ||
                (check == "1")) {
                answer = true;
            }
        }
        break;

    case INT:
        answer = (this->scalar_.int_ != 0);
        break;

    case UINT:
        answer = (this->scalar_.uint_ != 0);
        break;

    case LONG:
        answer = (this->scalar_.long_ != 0);
        break;

    case ULONG:
        answer = (this->scalar_.ulong_ != 0);
        break;

    case DOUBLE:
        answer = (std::fabs(this->scalar_.double_) > std::numeric_limits<double>::epsilon());
        break;

    default:
        break;
    }

    return answer;
}

int Variant::get_int() const {
    int answer = 0;
    switch (this->type()) {
    case BOOLEAN:
        answer = (this->scalar_.int_ != 0) ? 1 : 0;
        break;

    case STRING:
        answer = std::atoi(this->str_.c_str());
        break;

    case INT:
        answer = this->scalar_.int_;
        break;

    case UINT:
        answer = static_cast<int>(this->scalar_.uint_);
        break;

    case LONG:
        answer = static_cast<int>(this->scalar_.long_);
        break;

    case ULONG:
        answer = static_cast<int>(this->scalar_.ulong_);
        break;

    case DOUBLE:
        answer = static_cast<int>(this->scalar_.double_);
        break;

    default:
        break;
    }

    return answer;
}

unsigned int Variant::get_uint() const {
    unsigned int answer = 0;
    switch (this->type()) {
    case BOOLEAN:
        answer = (this->scalar_.int_ != 0) ? 1 : 0;
        break;

    case STRING:
        answer = std::atoi(this->str_.c_str());
        break;

    case INT:
        answer = static_cast<unsigned int>(this->scalar_.int_);
        break;

    case UINT:
        answer = this->scalar_.uint_;
        break;

    case LONG:
        answer = static_cast<unsigned int>(this->scalar_.long_);
        break;

    case ULONG:
        answer = static_cast<unsigned int>(this->scalar_.ulong_);
        break;

    case DOUBLE:
        answer = static_cast<unsigned int>(this->scalar_.double_);
        break;

    default:
        break;
    }

    return answer;
}

long Variant::get_long() const {
    long answer = 0;
    switch (this->type()) {
    case BOOLEAN:
        answer = (this->scalar_.int_ != 0) ? 1 : 0;
        break;

    case STRING:
        answer = std::atol(this->str_.c_str());
        break;

    case INT:
        answer = static_cast<long>(this->scalar_.int_);
        break;

    case UINT:
        answer = static_cast<long>(this->scalar_.uint_);
        break;

    case LONG:
        answer = this->scalar_.long_;
        break;

    case ULONG:
        answer = static_cast<long>(this->scalar_.ulong_);
        break;

    case DOUBLE:
        answer = static_cast<long>(this->scalar_.double_);
        break;

    default:
        break;
    }

    return answer;
}

unsigned long Variant::get_ulong() const {
    unsigned long answer = 0;
    switch (this->type()) {
    case BOOLEAN:
        answer = (this->scalar_.int_ != 0) ? 1 : 0;
        break;

    case STRING:
        answer = std::atol(this->str_.c_str());
        break;

    case INT:
        answer = static_cast<unsigned long>(this->scalar_.int_);
        break;

    case UINT:
        answer = static_cast<unsigned long>(this->scalar_.uint_);
        break;

    case LONG:
        answer = static_cast<unsigned long>(this->scalar_.long_);
        break;

    case ULONG:
        answer = this->scalar_.ulong_;
        break;

    case DOUBLE:
        answer = static_cast<unsigned long>(this->scalar_.double_);
        break;

    default:
        // do nothing
        break;
    }

    return answer;
}

double Variant::get_double() const {
    double answer = 0.0;
    switch (this->type()) {
    case BOOLEAN:
        answer = (this->scalar_.int_ != 0) ? 1.0 : 0.0;
        break;

    case STRING:
        answer = std::atof(this->str_.c_str());
        break;

    case INT:
        answer = static_cast<double>(this->scalar_.int_);
        break;

    case UINT:
        answer = static_cast<double>(this->scalar_.uint_);
        break;

    case LONG:
        answer = static_cast<double>(this->scalar_.long_);
        break;

    case ULONG:
        answer = static_cast<double>(this->scalar_.ulong_);
        break;

    case DOUBLE:
        answer = this->scalar_.double_;
        break;

    default:
        // do nothing
        break;
    }

    return answer;
}

std::string Variant::get_str() const {
    std::string answer = "";
    switch (this->type()) {
    case BOOLEAN:
        answer = (this->scalar_.int_ != 0) ? "true" : "false";
        break;

    case STRING:
        answer = this->str_;
        break;

    case INT:
        answer = this->x2s(this->scalar_.int_);
        break;

    case UINT:
        answer = this->x2s(this->scalar_.uint_);
        break;

    case LONG:
        answer = this->x2s(this->scalar_.long_);
        break;

    case ULONG:
        answer = this->x2s(this->scalar_.ulong_);
        break;

    case DOUBLE:
        answer = this->x2s(this->scalar_.double_);
        break;

    default:
        break;
    }

    return answer;
}

bool Variant::operator==(const Variant& rhs) const {
    bool answer = false;
    if (this->type() == rhs.type()) {
        switch (this->type()) {
        case BOOLEAN:
        case INT:
            answer = (this->scalar_.int_ == rhs.scalar_.int_);
            break;

        case UINT:
            answer = (this->scalar_.uint_ == rhs.scalar_.uint_);
            break;

        case LONG:
            answer = (this->scalar_.long_ == rhs.scalar_.long_);
            break;

        case ULONG:
            answer = (this->scalar_.ulong_ == rhs.scalar_.ulong_);
            break;

        case DOUBLE:
            answer = (std::fabs(this->scalar_.double_ - rhs.scalar_.double_) < std::numeric_limits<double>::epsilon());
            break;

        case STRING:
            answer = (this->str_ == rhs.str_);
            break;

        case ARRAY:
            if (this->size() == rhs.size()) {
                answer = true;
                const std::size_t maxSize = this->size();
                for (std::size_t i = 0; i < maxSize; ++i) {
                    if (this->getAt(i) != rhs.getAt(i)) {
                        answer = false;
                        break;
                    }
                }
            }
            break;

        case MAP:
            if (this->size() == rhs.size()) {
            }
            break;

        default:
            answer = true;
        }
    }

    return answer;
}

std::string Variant::str() const {
    std::string ans = "";

    switch (this->type()) {
    case ARRAY:
        {
            ans += "[";
            ArrayConstIterator pEnd = this->endArray();
            for (ArrayConstIterator p = this->beginArray(); p != pEnd; ++p) {
                const std::string item = p->str();
                ans += '"' + item + '"';
            }
            ans = ans.substr(0, ans.size() -2); // remove ", ".
            ans += "]";
        }
        break;

    case MAP:
        {
            ans += "{";
            MapConstIterator pEnd = this->endMap();
            for (MapConstIterator p = this->beginMap(); p != pEnd; ++p) {
                const std::string key = p->first.str();
                const std::string value = p->second.str();
                ans += "\"" + key + "\": \"" + value + "\", ";
            }
            ans = ans.substr(0, ans.size() -2); // remove ", ".
            ans += "}";
        }
        break;

    default:
        {
            ans = this->get_str();
        }
        break;
    }

    return ans;
}


void Variant::set(const bool value) {
    this->type_ = BOOLEAN;
    this->scalar_.int_ = (value == true) ? 1 : 0;
}

void Variant::set(const char value) {
    this->type_ = INT;
    this->scalar_.int_ = value;
}

void Variant::set(const unsigned char value) {
    this->type_ = UINT;
    this->scalar_.uint_ = value;
}

void Variant::set(const int value) {
    this->type_ = INT;
    this->scalar_.int_ = value;
}

void Variant::set(const unsigned int value) {
    this->type_ = UINT;
    this->scalar_.uint_ = value;
}

void Variant::set(const long value) {
    this->type_ = LONG;
    this->scalar_.long_ = value;
}

void Variant::set(const unsigned long value) {
    this->type_ = ULONG;
    this->scalar_.ulong_ = value;
}

void Variant::set(const double value) {
    this->type_ = DOUBLE;
    this->scalar_.double_ = value;
}

void Variant::set(const char* pStr) {
    this->set(std::string(pStr));
}

void Variant::set(const char* pStr, const std::size_t size) {
    this->set(std::string(pStr, size));
}

void Variant::set(const std::string& value) {
    this->type_ = STRING;
    this->str_ = value;
}

// ========================================================================
// operation
// ========================================================================
void Variant::merge(const Variant& rhs) {
    if (rhs.type() == ARRAY) {
        ArrayConstIterator pEnd = rhs.endArray();
        for (ArrayConstIterator p = rhs.beginArray(); p != pEnd; ++p) {
            this->push_back(*p);
        }
    } else if (rhs.type() == MAP) {
        MapConstIterator pEnd = rhs.endMap();
        for (MapConstIterator p = rhs.beginMap(); p != pEnd; ++p) {
            Variant key = p->first;
            Variant value = p->second;
            (*this)[key].merge(value);
        }
    } else {
        this->operator=(rhs);
    }
}


// ========================================================================
// protected
// ========================================================================
void Variant::clearChildren() {
    for (ArrayContainerType::iterator p = this->array_.begin(); p != this->array_.end(); ++p) {
        delete *p;
        *p = NULL;
    }
    this->array_.clear();

    for (MapContainerType::iterator p = this->map_.begin(); p != this->map_.end(); ++p) {
        delete p->first;
        delete p->second;
        p->second = NULL;
    }
    this->map_.clear();
}

void Variant::copyChildren(const Variant& rhs) {
    assert(this->array_.size() == 0);
    if (rhs.array_.empty() != true) {
        this->array_.reserve(rhs.array_.size());
        for (ArrayContainerType::const_iterator p = rhs.array_.begin(); p != rhs.array_.end(); ++p) {
            Variant* pNew = new Variant(*(*p));
            this->array_.push_back(pNew);
        }
    }

    assert(this->map_.size() == 0);
    if (rhs.map_.empty() != true) {
        for (MapContainerType::const_iterator p = rhs.map_.begin(); p != rhs.map_.end(); ++p) {
            Variant* pKey = new Variant(*(p->first));
            Variant* pValue = new Variant(*(p->second));
            this->map_.insert(std::pair<Variant*, Variant*>(pKey, pValue));
        }
    }
}

const Variant& Variant::getNullObject() {
    if (Variant::pNullObject_ == NULL) {
        Variant::pNullObject_ = new Variant;
    }
    return *(Variant::pNullObject_);
}


#endif // VARIANT_H
