#include "RelationshipCache.h"
#include "OnDemandExtractor.h"

OnDemandExtractor RelationshipCache::ode;

RelationshipCache::RelationshipCache(RELATIONSHIP_TYPE rType) : RelationshipStorage(), 
populated(false), isPopulating(false) {
	this->rType = rType;
}

void RelationshipCache::setPopulated(bool populated) {
	this->populated = populated;
}

bool RelationshipCache::isPopulated() {
	return populated;
}

void RelationshipCache::clear() {
	RelationshipStorage::clear();
	populated = false;
	populatedValues.clear();
	reversePopulatedValues.clear();
	isPopulating = false;
}

void RelationshipCache::addPopulatedValue(INDEX populatedValue) {
	populatedValues.insert(populatedValue);
}

void RelationshipCache::addReversePopulatedValue(INDEX reversePopulatedValue) {
	reversePopulatedValues.insert(reversePopulatedValue);
}

bool RelationshipCache::isValuePopulated(INDEX value) {
	return populatedValues.count(value);
}

bool RelationshipCache::isValueReversePopulated(INDEX value) {
	return reversePopulatedValues.count(value);
}

INDICES& RelationshipCache::getFirstValues() {
	populateCache();
	return RelationshipStorage::getFirstValues();
}

INDICES& RelationshipCache::getSecondValues() {
	populateCache();
	return  RelationshipStorage::getSecondValues();
}

PAIRS<INDEX> RelationshipCache::getValuePairs() {
	populateCache();
	return  RelationshipStorage::getValuePairs();
}

INDICES& RelationshipCache::getFirstValuesOf(INDEX secondValue) {
	rType == RELATIONSHIP_TYPE::AFFECTS_STAR ? populateReverseCache(secondValue) : populateCache();
	return RelationshipStorage::getFirstValuesOf(secondValue);
}

INDICES& RelationshipCache::getSecondValuesOf(INDEX firstValue) {
	switch (rType) {
		case RELATIONSHIP_TYPE::NEXT_STAR :
		case RELATIONSHIP_TYPE::AFFECTS_STAR :
			populateCache(firstValue);
			break;
		case RELATIONSHIP_TYPE::AFFECTS:
			populateCache();
			break;
		default:
			break;
	}
	return  RelationshipStorage::getSecondValuesOf(firstValue);
}

bool RelationshipCache::hasRelationship(INDEX firstValue, INDEX secondValue) {
	switch (rType) {
	case RELATIONSHIP_TYPE::NEXT_STAR:
	case RELATIONSHIP_TYPE::AFFECTS_STAR:
		populateCache(firstValue);
		break;
	case RELATIONSHIP_TYPE::AFFECTS:
		populateCache();
		break;
	default:
		break;
	}

	return  RelationshipStorage::hasRelationship(firstValue, secondValue);
}

bool RelationshipCache::hasRelationshipWithSecondValue(INDEX secondValue) {
	rType == RELATIONSHIP_TYPE::AFFECTS_STAR ? populateReverseCache(secondValue) : populateCache();
	return  RelationshipStorage::hasRelationshipWithSecondValue(secondValue);
}

bool RelationshipCache::hasRelationshipWithFirstValue(INDEX firstValue) {
	switch (rType) {
	case RELATIONSHIP_TYPE::NEXT_STAR:
	case RELATIONSHIP_TYPE::AFFECTS_STAR:
		populateCache(firstValue);
		break;
	case RELATIONSHIP_TYPE::AFFECTS:
		populateCache();
		break;
	default:
		break;
	}

	return  RelationshipStorage::hasRelationshipWithFirstValue(firstValue);
}

void RelationshipCache::populateCache(INDEX value) {
	if (isPopulating || isValuePopulated(value)) return;

	isPopulating = true;
	switch (rType) {
	case RELATIONSHIP_TYPE::NEXT_STAR:
		ode.extractNextStar(value, *this);
		break;
	case RELATIONSHIP_TYPE::AFFECTS_STAR:
		ode.extractsAffectsStar(value, *this);
		break;
	default:
		break;
	}
	isPopulating = false;
}

void RelationshipCache::populateReverseCache(INDEX value) {
	if (isPopulating || isValueReversePopulated(value)) return;

	isPopulating = true;
	switch (rType) {
	case RELATIONSHIP_TYPE::AFFECTS_STAR:
		ode.extractsReverseAffectsStar(*this, value);
		addReversePopulatedValue(value);
		break;
	default:
		break;
	}
	isPopulating = false;
}

void RelationshipCache::populateCache() {

	if (isPopulating || isPopulated()) return;

	isPopulating = true;
	switch (rType) {
	case RELATIONSHIP_TYPE::NEXT_STAR:
		ode.extractNextStar(*this);
		break;
	case RELATIONSHIP_TYPE::AFFECTS:
		ode.extractAffects(*this);
		break;
	case RELATIONSHIP_TYPE::AFFECTS_STAR:
		ode.extractAffectsStar(*this);
		break;
	default:
		break;
	}
	populated = true;
	isPopulating = false;
}
