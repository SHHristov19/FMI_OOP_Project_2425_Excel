#pragma once
#include <iostream>

template <typename T>
class container
{
private:
	T** data = nullptr;
	size_t size = 0;
	size_t capacity = 0;

	explicit container(size_t size);

	void resize(size_t newCapacity);

	unsigned int getNextPowerOfTwo(unsigned int n) const;
	unsigned int allocateCapacity(unsigned int n) const;

	void copyFrom(const container<T>& other);
	void moveFrom(container<T>&& other) noexcept;
	

public:
	container();

	container(const container<T>& other);
	container(container<T>&& other) noexcept;

	container<T>& operator=(const container<T>& other);
	container<T>& operator=(container<T> && other) noexcept;
	const T* operator[](size_t idx) const;
	T*& operator[](size_t idx);

	size_t getSize() const;
	size_t getCapacity() const;
	bool contains(const T* ptr) const { return std::find(data, data + size, ptr) != data + size; }

	void push_back(T* ptr);

	void free();
	~container() noexcept;
};

template<typename T>
container<T>::container() : container(4) {}

template<typename T>
container<T>::container(size_t size) : size(0)
{
	this->capacity = allocateCapacity(size);
	this->data = new T*[this->capacity] { nullptr };
}

template<typename T>
container<T>::container(const container<T>& other)
{
	copyFrom(other);
}

template<typename T>
container<T>& container<T>::operator=(const container<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

template<typename T>
container<T>::container(container<T>&& other) noexcept
{
	moveFrom(std::move(other));
}

template<typename T>
container<T>& container<T>::operator=(container<T>&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}

	return *this;
}

template<typename T>
size_t container<T>::getSize() const
{
	return this->size;
}

template<typename T>
size_t container<T>::getCapacity() const
{
	return this->capacity;
}

template<typename T>
void container<T>::push_back(T* ptr)
{
	if (getSize() >= getCapacity())
	{
		resize(getCapacity() * 2);
	}

	this->data[this->size++] = ptr;
}

template<typename T>
const T* container<T>::operator[](size_t idx) const
{
	return this->data[idx];
}

template<typename T>
T*& container<T>::operator[](size_t idx) 
{
	return this->data[idx];
}

template<typename T>
container<T>::~container() noexcept
{
	free();
}

template<typename T>
unsigned int container<T>::getNextPowerOfTwo(unsigned int n) const
{
	if (n == 0) return 1;

	while (n & (n - 1))
	{
		n &= (n - 1);
	}

	return n << 1;
}

template<typename T>
unsigned int container<T>::allocateCapacity(unsigned int n) const
{
	return std::max(getNextPowerOfTwo(n + 1), 8u);
}

template<typename T>
void container<T>::resize(size_t newCapacity)
{
	T** newData = new T*[newCapacity] { nullptr };

	for (size_t i = 0; i < this->size; i++)
	{
		newData[i] = this->data[i];
	}

	delete[] this->data;
	this->data = newData;
	this->capacity = newCapacity;
}

template<typename T>
void container<T>::copyFrom(const container<T>& other)
{
	this->size = other.size;
	this->capacity = other.capacity;

	this->data = new T*[this->capacity] { nullptr };

	for (size_t i = 0; i < this->size; i++)
	{
		this->data[i] = other.data[i] ? other.data[i]->clone() : nullptr;
	}
}

template<typename T>
void container<T>::moveFrom(container<T>&& other) noexcept
{
	this->data = other.data;
	this->size = other.size;
	this->capacity = other.capacity;

	other.data = nullptr;
	other.size = other.capacity = 0;
}

template<typename T>
void container<T>::free()
{
	for (size_t i = 0; i < getSize(); i++)
	{
		if (this->data[i] != nullptr) {
			delete this->data[i];
			this->data[i] = nullptr;
		}
	}

	delete[] this->data;

	this->data = nullptr;
	this->size = this->capacity = 0;
}
