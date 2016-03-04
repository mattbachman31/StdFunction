#ifndef FUNC_H
#define FUNC_H

#include <iostream>
#include <functional>
#include <memory>

namespace cs540{
	class BadFunctionCall : public std::bad_function_call{

	};

	template <typename>
	class Function;

	template <typename ResultType, typename ... ArgumentTypes> 
	class Function<ResultType(ArgumentTypes...)>{
		private:

			struct BaseInner{
				virtual ResultType operator()(ArgumentTypes... args) = 0;

				virtual BaseInner* clone(){
					return nullptr;
				}

				virtual ~BaseInner(){}
			};

			template<typename T>
			struct Inner : BaseInner{
				Inner(T ft) : fp(ft){}

				ResultType operator()(ArgumentTypes... args){
					return fp(args...);
				}

				BaseInner* clone(){
					return new Inner<T>(fp);
				}

				~Inner(){}

				T fp;
			};

			BaseInner* functor;

		public:
			Function(){
				functor = nullptr;
			}

			template <typename FunctionType>
			Function(FunctionType ft) : functor(new Inner<FunctionType>(ft)){}

			Function(const Function &f){
				functor = f.functor->clone();
			}

			Function& operator=(const Function &f){
				if(&f != this){
					if(functor != nullptr){
						delete functor;
					}
					functor = f.functor->clone();
				}
				return (*this);
			}

			~Function(){
				delete functor;
			}

			ResultType operator()(ArgumentTypes... args){
				ResultType ret;
				if(functor == nullptr){
					throw BadFunctionCall();
				}
				try{
					ret =  (*functor)(args...);
				}
				catch(std::bad_function_call&){
					throw BadFunctionCall();
				}
				return ret;
			}

			explicit operator bool() const{
				return !(functor == nullptr);
			}

	};

	template <typename ResultType, typename... ArgumentTypes>
	bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){
		return !bool(f);
	}

	template <typename ResultType, typename... ArgumentTypes>
	bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> & f){
		return !bool(f);
	}

	template <typename ResultType, typename... ArgumentTypes>
	bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t){
		return bool(f);
	}

	template <typename ResultType, typename... ArgumentTypes>
	bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f){
		return bool(f);
	}
}

#endif