
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
};

