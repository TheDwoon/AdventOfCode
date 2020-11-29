#pragma once

namespace math {
	template<int N, typename T>
	class vec {
	public:
		T m_values[N];

		template<typename... T>
		vec(T... t) : m_values{ t... } {};
	};
}