#include<sstream>
#include<string>
#include<typeinfo>

template<typename data_t>
class Dyn_array
{
private:
	data_t *m_data_ptr;
	size_t m_capacity;
	size_t m_len;



	void grow_if_needed()
	{
		// The "-1" is to give some headroom, and for a simpler dislocate_right
		if(m_len >= m_capacity - 1) grow();
	}



	void grow()
	{
		realloc_data(m_capacity * 2);
	}



	void shrink_if_needed()
	{
		if(m_capacity > 16 && m_len <= m_capacity / 4) shrink();
	}



	void shrink()
	{
		realloc_data(m_capacity / 2);
	}



	void realloc_data(size_t new_capacity)
	{
		data_t *new_data_ptr = new data_t[new_capacity];
		copy_content(m_data_ptr, new_data_ptr, m_len);
		delete[] m_data_ptr;
		m_data_ptr = new_data_ptr;
		m_capacity = new_capacity;
	}



	void copy_content(data_t *from, data_t *to, size_t content_len)
	{
		for(size_t i = 0; i < content_len; ++i){
			to[i] = from[i];
		}
	}



	void dislocate_right(size_t from_index)
	{
		// To make sure we have the space for the last item
		++m_len;
		grow_if_needed();
		--m_len;

		if(from_index + 1 > m_len) std::length_error("Cannot non-continuosly dislocate");

		// Casting to int64_t is needed cause size_t is unsigned
		// and thus, dislocating from index 0 causes it to underflow
		for(int64_t i = static_cast<int64_t>(m_len - 1);
			i >= static_cast<int64_t>(from_index);
			--i
		){
			m_data_ptr[i + 1] = m_data_ptr[i];
		}
	
		m_data_ptr[from_index] = 0;
		m_len++;
	}



	void dislocate_left(size_t from_index)
	{
		if(from_index + 1 > m_len) std::length_error("Cannot non-continuosly dislocate");

		for(size_t i = from_index; i < m_len; ++i){
			m_data_ptr[i] = m_data_ptr[i + 1];
		}
	
		--m_len;
		shrink_if_needed();
	}



public:
	Dyn_array(size_t initial_size = 16) :
		m_data_ptr{new data_t[initial_size]},
		m_capacity{initial_size},
		m_len{0}
	{
		return;
	}



	~Dyn_array()
	{
		delete[] m_data_ptr;
		m_len = 0;
		m_capacity = 0;
	}



	data_t at(int64_t position)
	{
		position = position < 0 ? static_cast<int64_t>(m_len) + position : position;

		// Checking for position < 0 in case position is less than -m_len
		if(position >= static_cast<int64_t>(m_len) || position < 0){
			throw std::length_error("Cannot access item at position outside array");
		}
		return m_data_ptr[position];
	}


	std::string as_string()
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



	inline size_t get_len()
	{
		return m_len;
	}



	inline size_t get_capacity()
	{
		return m_capacity;
	}



	inline bool is_empty()
	{
		return m_len == 0;
	}
};

