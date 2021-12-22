#include "..\Headers\Navigation.h"

#include "Cell.h"

CNavigation::CNavigation(DEVICES)
	:CComponent(pDevice, pDeviceContext)
{
}

CNavigation::CNavigation(const CNavigation & other)
	:CComponent(other)
	, m_Cells(other.m_Cells)
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
}

HRESULT CNavigation::Initialize_Prototype(const _tchar * pDataFiles)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pDataFiles, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	ReadFile(hFile, &m_iNumCells, sizeof(_uint), &dwByte, nullptr);

	m_Cells.reserve(m_iNumCells);

	for (_uint i = 0; i < m_iNumCells; ++i)
	{
		_vector		vPoints[CCell::POINT_END];

		ReadFile(hFile, vPoints, sizeof(_vector) * CCell::POINT_END, &dwByte, nullptr);

		CCell*		pCell = CCell::Create(m_pDevice, m_pDeviceContext, vPoints, (_int)m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);



	if (FAILED(SetUp_NeighborCell()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::Initialize_Clone(void * pArg)
{
	if (nullptr != pArg)
		m_NaviDesc = *(NAVIDESC*)pArg;

	return S_OK;
}

_bool CNavigation::isMove(_fvector vPoint)
{
	CCell*		pNeighborCell = nullptr;

	if (true == m_Cells[m_NaviDesc.iCurrentCellIndex]->isIn(vPoint, &pNeighborCell))
	{
		return true;
	}
	else
	{
		if (nullptr != pNeighborCell)
		{
			while (true)
			{
				if (nullptr == pNeighborCell)
					return false;

				if (true == pNeighborCell->isIn(vPoint, &pNeighborCell))
				{
					m_NaviDesc.iCurrentCellIndex = pNeighborCell->Get_Index();
					break;
				}
			}
			return true;
		}
		else
			return false;
	}

	return _bool();
}

#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
			pCell->Render();
	}

	return S_OK;
}

#endif

HRESULT CNavigation::SetUp_NeighborCell()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
		}
	}

	return S_OK;
}

CNavigation * CNavigation::Create(DEVICES, const _tchar * pDataFiles)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize_Prototype(pDataFiles)))
	{
		MSG_BOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*		pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize_Clone(pArg)))
	{
		MSG_BOX("Failed to Creating CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
	m_Cells.clear();
}
