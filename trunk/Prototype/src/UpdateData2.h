#ifndef __UpdateData2_h__
#define __UpdateData2_h__

namespace Prototype
{

	template<typename T1, typename T2>
	struct UpdateData2
	{
		T1 v1;
		T2 v2;
		inline UpdateData2()			{}
		inline UpdateData2(const T1 &v1, const T2 &v2)
			: v1(v1), v2(v2)
		{}

		UpdateData2<T1, T2> operator +(const UpdateData2<T1, T2> &rh) const
		{
			UpdateData2<T1, T2> result(v1 + rh.v1, v2 + rh.v2);
			return result;
		}

		UpdateData2<T1, T2> operator -(const UpdateData2<T1, T2> &rh) const
		{
			UpdateData2<T1, T2> result(v1 - rh.v1, v2 - rh.v2);
			return result;
		}

		UpdateData2<T1, T2> operator *(float rh) const
		{
			UpdateData2<T1, T2> result(v1 * rh, v2 * rh);
			return result;
		}

		bool operator ==(const UpdateData2<T1, T2> &rh) const
		{				
			return (v1 == rh.v1) && (v2 == rh.v2);
		}

		bool operator !=(const UpdateData2<T1, T2> &rh) const
		{				
			return (v1 != rh.v1) && (v2 != rh.v2);
		}

		const UpdateData2<T1, T2>& operator =(const UpdateData2<T1, T2> &rh)
		{
			this->v1 = rh.v1;
			this->v2 = rh.v2;
			return *this;
		}
	};
	
};

#endif