#include <cassert>
#include <LMM/pricer/McLmmVanillaCapletPricer.h>

double McLmmVanillaCapletPricer::price(const VanillaCaplet& vanillaCaplet, size_t nbSimulation)  const
{
	assert( mcLmm_->get_lmm()->get_LMMTenorStructure()->get_tenorType() == vanillaCaplet.get_lmmTenorStructureTenorType()); // this pricer can price this Caplet

	//VanillaSwap vanillaSwap = vanillaSwaption.getUnderlyingSwap();
	//! YY TODO: need to implement the == operator for enum TenorType	

	double result   = 0.0;
	double variance = 0.0;

	LMM::Index indexValuationdate = 0;
	LMM::Index indexPayment = vanillaCaplet.get_indexPayment();
	LMM::Index indexLibor    = indexPayment - 1;

	for(size_t itrSimulation=0; itrSimulation<nbSimulation; ++itrSimulation)
	{
		mcLmm_->simulateLMM(); // YY TODO: not efficient at all, don't need to do all the simulation ... 
		const matrix& liborMatrix  = mcLmm_->get_liborMatrix();
		const std::vector<double>& numeraire = mcLmm_->get_numeraire();

		double liborRate    =  liborMatrix(indexLibor, indexLibor);
		double payoff =  vanillaCaplet.payoff(liborRate);
		double value  =  payoff * numeraire[indexValuationdate] / numeraire[indexPayment];
 
		result	 += value;
		variance += value*value;
	}
	result/=nbSimulation; 
	
	if(LMM::DEUBGLMM())
	{
		variance = (variance/nbSimulation-result*result);	
		std::cout << LMM::NOTIF_MSG 
			<< " mcScheme=" << MCSchemeType::mcSchemeType2String(mcLmm_->get_mcSchemeType())
			<< ", nbSimualtion = " << nbSimulation
			<< ", SwaptionPriceMC=" << result
			<< ",  99% confidential interval = " << 2.57*std::sqrt(variance / nbSimulation) << std::endl;
	}

	return result;
}
