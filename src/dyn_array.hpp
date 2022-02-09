#include<sstream>
#include<string>
#include<typeinfo>

template<typename data_t>
class Dyn_array
{
private:
	data_t *m_data_ptr = nullptr;
	size_t m_capacity = 0;
	size_t m_len = 0;



	void grow_if_needed(size_t surplus_capacity = 0)
	{
		// The "-1" is to give some headroom, and for a simpler dislocate_right.
		if(m_len + surplus_capacity >= m_capacity) realloc_data(m_capacity * 2);
	}



	void shrink_if_needed(void)
	{
		// Using "m_capacity / 4" so we don't keep shrinking and growing when we are on the edge of
		// capacity.
		if(m_len <= m_capacity / 4 && m_capacity > 16) realloc_data(m_capacity / 2);
	}



	void copy_content(data_t *from, data_t *to, const size_t content_len)
	{
		for(size_t i = 0; i < content_len; ++i){
			to[i] = from[i];
		}
	}



	void realloc_data(const size_t new_capacity)
	{
		data_t *new_data_ptr = new data_t[new_capacity];

		copy_content(m_data_ptr, new_data_ptr, m_len);

		delete[] m_data_ptr;
		m_data_ptr = new_data_ptr;
		m_capacity = new_capacity;
	}



	void dislocate_right(const size_t from_index)
	{
		grow_if_needed(1);

		if(from_index + 1 > m_len) throw std::length_error("Cannot non-continuosly dislocate");

		// Casting to int64_t is needed cause size_t is unsigned
		// and thus, dislocating from index 0 causes it to underflow
		for(int64_t i = static_cast<int64_t>(m_len - 1);
			i >= static_cast<int64_t>(from_index);
			--i
		){
			m_data_ptr[i + 1] = m_data_ptr[i];
		}
	
		m_data_ptr[from_index] = 0;
		++m_len;
	}



	void dislocate_left(const size_t from_index)
	{
		if(from_index + 1 > m_len) throw std::length_error("Cannot non-continuosly dislocate");

		for(size_t i = from_index; i < m_len; ++i){
			m_data_ptr[i] = m_data_ptr[i + 1];
		}
	
		--m_len;
		shrink_if_needed();
	}



public:
	Dyn_array(const size_t initial_size = 16) :
		m_data_ptr{new data_t[initial_size]},
		m_capacity{initial_size},
		m_len{0}
	{}



	Dyn_array(const Dyn_array& other) :
		m_capacity{other.get_capacity()},
		m_len{other.get_len()}
	{
		m_data_ptr = new data_t[other.get_capacity()]; 
		copy_content(other.m_data_ptr, m_data_ptr, other.get_len());
	}



	~Dyn_array()
	{
		delete[] m_data_ptr;
		m_len = 0;
		m_capacity = 0;
	}


	const data_t& operator[](const size_t index) const
	{
		if(index >= m_len) throw std::length_error("Index out of range");
		return m_data_ptr[index];
	}



	data_t& operator[](const size_t index)
	{
		if(index >= m_len) throw std::length_error("Index out of range");
		return m_data_ptr[index];
	}



	void push(const data_t& value)
	{
		grow_if_needed();

		m_data_ptr[m_len] = value;
		++m_len;
	}



	void pop(void)
	{
		if(m_len == 0) throw std::length_error("Array already empty");
		m_data_ptr[m_len - 1].~data_t();
		--m_len;
		shrink_if_needed();
	}



	void insert(const data_t& value, const size_t index)
	{
		if(index > m_len){
			throw std::length_error("Index out of bounds");
		}
		if(index == m_len){
			push(value);
			return;
		}

		dislocate_right(index);
		m_data_ptr[index] = value;
	}



	void remove(const size_t index)
	{
		if(index >= m_len) throw std::length_error("Index out of bounds");
		if(index == m_len - 1){
			pop();
			return;
		}
		m_data_ptr[index].~data_t();
		dislocate_left(index);
		return;
	}



	const data_t& at(const int64_t index) const
	{
		auto actual_index = index < 0 ? static_cast<int64_t>(m_len) + index : index;
	
		// Checking for actual_index < 0 in case the original index < - m_len
		if(actual_index < 0) throw std::length_error("Index out of bounds");

		return (*this)[static_cast<size_t>(actual_index)];
	}



	std::string as_string(void) const
	{
		std::stringstream ret_buffer{""};

		//typeid(data_t).name() needs specific compiler flags to work
		ret_buffer << "dyn_array<" << typeid(data_t).name() << "> @ " << &m_data_ptr << "\n";

		ret_buffer << "\tlen: " << get_len() << "\n";

		ret_buffer << "\tdata: [";
		for (size_t i = 0; i < m_len; ++i){
			ret_buffer << m_data_ptr[i];
			if(i != m_len - 1){
				ret_buffer << ", "; 
			}

		}
		ret_buffer << "]";

		return ret_buffer.str();
	}



	size_t get_len(void) const
	{
		return m_len;
	}



	size_t get_capacity(void) const
	{
		return m_capacity;
	}



	bool is_empty() const
	{
		return m_len == 0;
	}
};
