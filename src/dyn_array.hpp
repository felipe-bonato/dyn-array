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

