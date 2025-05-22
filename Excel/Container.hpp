#pragma once
#include <iostream>

template <typename T>
class Container
{
private:
	T** data = nullptr;
	size_t size = 0;
	size_t capacity = 0;

	explicit Container(size_t size);

	void resize(size_t newCapacity);

	unsigned int getNextPowerOfTwo(unsigned int n) const;
	unsigned int allocateCapacity(unsigned int n) const;

	void copyFrom(const Container<T>& other);
	void moveFrom(Container<T>&& other) noexcept;
	void free();

public:
	Container();

	Container(const Container<T>& other);
	Container<T>& operator=(const Container<T>& other);

	Container(Container<T>&& other) noexcept;
	Container<T>& operator=(Container<T> && other) noexcept;

	size_t getSize() const;
	size_t getCapacity() const;

	void push_back(T* ptr);

	const T* operator[](size_t idx) const;
	T*& operator[](size_t idx);

	~Container() noexcept;
};

template<typename T>
Container<T>::Container() : Container(4) {}

template<typename T>
Container<T>::Container(size_t size) : size(0)
{
	this->capacity = allocateCapacity(size);
	this->data = new T*[this->capacity] { nullptr };
}

template<typename T>
Container<T>::Container(const Container<T>& other)
{
	copyFrom(other);
}

template<typename T>
Container<T>& Container<T>::operator=(const Container<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

template<typename T>
Container<T>::Container(Container<T>&& other) noexcept
{
	moveFrom(std::move(other));
}

template<typename T>
Container<T>& Container<T>::operator=(Container<T>&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}

	return *this;
}

template<typename T>
size_t Container<T>::getSize() const
{
	return this->size;
}

template<typename T>
size_t Container<T>::getCapacity() const
{
	return this->capacity;
}

template<typename T>
void Container<T>::push_back(T* ptr)
{
	if (getSize() >= getCapacity())
	{
		resize(getCapacity() * 2);
	}

	this->data[this->size++] = ptr;
}

template<typename T>
const T* Container<T>::operator[](size_t idx) const
{
	return this->data[idx];
}

template<typename T>
T*& Container<T>::operator[](size_t idx) {
	return this->data[idx];
}

template<typename T>
Container<T>::~Container() noexcept
{
	free();
}

template<typename T>
unsigned int Container<T>::getNextPowerOfTwo(unsigned int n) const
{
	if (n == 0) return 1;

	while (n & (n - 1))
	{
		n &= (n - 1);
	}

	return n << 1;
}

template<typename T>
unsigned int Container<T>::allocateCapacity(unsigned int n) const
{
	return std::max(getNextPowerOfTwo(n + 1), 8u);
}

template<typename T>
void Container<T>::resize(size_t newCapacity)
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
void Container<T>::copyFrom(const Container<T>& other)
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
void Container<T>::moveFrom(Container<T>&& other) noexcept
{
	this->data = other.data;
	this->size = other.size;
	this->capacity = other.capacity;

	other.data = nullptr;
	other.size = other.capacity = 0;
}

template<typename T>
void Container<T>::free()
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
