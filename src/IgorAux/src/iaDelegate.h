//
//   ______                                |\___/|  /\___/\
//  /\__  _\                               )     (  )     (
//  \/_/\ \/       __      ___    _ __    =\     /==\     /=
//     \ \ \     /'_ `\   / __`\ /\`'__\    )   (    )   (
//      \_\ \__ /\ \L\ \ /\ \L\ \\ \ \/    /     \   /   \
//      /\_____\\ \____ \\ \____/ \ \_\   |       | /     \
//  ____\/_____/_\/___L\ \\/___/___\/_/____\__  _/__\__ __/________________
//                 /\____/                   ( (       ))
//                 \_/__/  game engine        ) )     ((
//                                           (_(       \)
// (c) Copyright 2014-2015 by Martin Loga
//
// This library is free software; you can redistribute it and or modify it   
// under the terms of the GNU Lesser General Public License as published by  
// the Free Software Foundation; either version 3 of the License, or (at   
// your option) any later version.                                           
// 
// This library is distributed in the hope that it will be useful,           
// but WITHOUT ANY WARRANTY; without even the implied warranty of            
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         
// Lesser General Public License for more details.                           
// 
// You should have received a copy of the GNU General Public License
// along with this program.If not, see <http://www.gnu.org/licenses/>.
// 
// contact: martinloga@gmx.de  

#ifndef __IGOR_AUX_DELEGATE__
#define __IGOR_AUX_DELEGATE__

namespace IgorAux
{

	/*! delegate
	*/
	#define iaDELEGATE(DelegateName,ReturnType,ParameterList,InnerParameterList) \
	class DelegateName\
	{\
	protected:\
		class InternalBase\
		{\
		public:\
			virtual ReturnType Execute ParameterList const = 0;\
			virtual InternalBase *Clone() const = 0;\
			virtual bool Compare(const InternalBase *fpDelegateInternalBase) const = 0;\
			virtual int GetType() const = 0;\
		};\
	\
		template<typename T>class InternalThisCall:public InternalBase\
		{\
		public:\
			__IGOR_MEMBERFUNCTION_POINTER__(T, MemberFunctionPointer, ReturnType, ParameterList);\
	\
			InternalThisCall(T *fpInstance, MemberFunctionPointer fpMemberFunction)\
			{\
				this->memberFunction = fpMemberFunction;\
				this->instance = fpInstance;\
			}\
	\
			virtual ReturnType Execute ParameterList const\
			{\
				return (instance->*memberFunction) InnerParameterList;\
			}\
	\
			virtual InternalBase *Clone() const\
			{\
				InternalThisCall<T> *clone = (InternalThisCall<T>*)malloc(sizeof(InternalThisCall<T>));\
				new(clone) InternalThisCall<T>((T*)this->instance, this->memberFunction);\
				return clone;\
			}\
	\
			virtual bool Compare(const InternalBase *fpDelegateInternalBase) const\
			{\
				if (this->GetType() != fpDelegateInternalBase->GetType()) return false;\
				return (this->instance == ((InternalThisCall<T>*)fpDelegateInternalBase)->instance) &&\
					(this->memberFunction == ((InternalThisCall<T>*)fpDelegateInternalBase)->memberFunction);\
			}\
	\
			virtual int GetType() const { return 0; }\
	\
		protected:\
			MemberFunctionPointer memberFunction;\
			T *instance;\
		};\
	\
		class InternalDefaultCall:public InternalBase\
		{\
		public:\
			__IGOR_FUNCTION_POINTER__(FunctionPointer, __IGOR_DEFAULTCALL__, ReturnType, ParameterList);\
	\
			InternalDefaultCall(FunctionPointer fpFunction):function(fpFunction) {}\
	\
			virtual ReturnType Execute ParameterList const\
			{\
				return function InnerParameterList;\
			}\
	\
			virtual InternalBase *Clone() const\
			{\
				InternalDefaultCall *clone = (InternalDefaultCall*)malloc(sizeof(InternalDefaultCall));\
				new(clone) InternalDefaultCall(this->function);\
				return clone;\
			}\
	\
			virtual bool Compare(const InternalBase *fpDelegateInternalBase) const\
			{\
				if (this->GetType() != fpDelegateInternalBase->GetType()) return false;\
				return (this->function == ((InternalDefaultCall*)fpDelegateInternalBase)->function);\
			}\
	\
			virtual int GetType() const { return 1; }\
		protected:\
			FunctionPointer function;\
		};\
	public:\
		DelegateName():delegateInternal(NULL) {}\
		DelegateName(const DelegateName &fpDelegate):delegateInternal(fpDelegate.delegateInternal->Clone()) {}\
	\
		DelegateName(InternalDefaultCall::FunctionPointer fpFunction):delegateInternal(NULL)\
		{\
			this->delegateInternal = (InternalDefaultCall*)malloc(sizeof(InternalDefaultCall));\
			new(this->delegateInternal) InternalDefaultCall(fpFunction);\
		}\
	\
		template<typename T> DelegateName(T *fpInstance, typename InternalThisCall<T>::MemberFunctionPointer fpMemberFunction)\
		{\
			this->delegateInternal = (InternalThisCall<T>*)malloc(sizeof(InternalThisCall<T>));\
			new(this->delegateInternal) InternalThisCall<T>(fpInstance, fpMemberFunction);\
		}\
	\
		~DelegateName()\
		{\
			if (this->delegateInternal != NULL)\
			{\
				this->delegateInternal->~InternalBase();\
				free(this->delegateInternal);\
				this->delegateInternal = NULL;\
			}\
		}\
	\
		const DelegateName &operator=(const DelegateName &fpDelegate)\
		{\
			if (this->delegateInternal != NULL)\
			{\
				this->delegateInternal->~InternalBase();\
				free(this->delegateInternal);\
				this->delegateInternal = NULL;\
			}\
			this->delegateInternal = fpDelegate.delegateInternal->Clone();\
			return *this;\
		}\
	\
		bool operator==(const DelegateName &fpDelegate) const\
		{\
			if (this->delegateInternal == NULL) return (fpDelegate.delegateInternal == NULL);\
			return (this->delegateInternal->Compare(fpDelegate.delegateInternal));\
		}\
	\
		ReturnType operator()ParameterList const\
		{\
			if (delegateInternal == NULL)\
			{\
				typedef ReturnType DummyType; return DummyType();\
			}\
			return delegateInternal->Execute InnerParameterList;\
		}\
	\
	protected:\
		InternalBase *delegateInternal;\
	};

};

#endif