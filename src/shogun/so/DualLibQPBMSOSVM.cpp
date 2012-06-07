/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2012 Michal Uricar
 * Copyright (C) 2012 Michal Uricar
 */

#include <shogun/so/DualLibQPBMSOSVM.h>
#include <shogun/so/libbmrm.h>

using namespace shogun;

CDualLibQPBMSOSVM::CDualLibQPBMSOSVM()
:CLinearStructuredOutputMachine()
{
}

CDualLibQPBMSOSVM::CDualLibQPBMSOSVM(
		CStructuredModel* 	model,
		CLossFunction* 		loss,
		CStructuredLabels*	labs,
		CFeatures*			features,
		float64_t           lambda)
:CLinearStructuredOutputMachine(model, loss, labs, features)
{
	set_opitons(0.001, 0.0, 100);
	set_lambda(lambda);
}

CDualLibQPBMSOSVM::~CDualLibQPBMSOSVM()
{
}

void CDualLibQPBMSOSVM::set_opitons(float64_t TolRel, float64_t TolAbs, uint32_t BufSize)
{
	m_TolRel=TolRel;
	m_TolAbs=TolAbs;
	m_BufSize=BufSize;
}

bool CDualLibQPBMSOSVM::train_machine(CFeatures* data)
{
	// get dimension of w
	uint32_t nDim=m_risk_function->get_w_dim(data);

	// call the BMRM solver
	bmrm_return_value_T result = svm_bmrm_solver(data, m_w.vector, m_TolRel, m_TolAbs, m_lambda,
			m_BufSize, nDim, m_risk_function);

	if (result.exitflag==1)
	{
		return true;
	} else {
		return false;
	}
}

void CDualLibQPBMSOSVM::register_parameters()
{
	SG_ADD(&m_w, "m_w", "Weight vector", MS_NOT_AVAILABLE);
}

