#pragma once

class Action {
public:
	Action(std::function<void(void)> howToDo, std::function<void(void)> howToUndo) 
		: m_howToDo(howToDo), m_howToUndo(howToUndo)
	{}
	Action() = default;
	~Action() = default;
	inline void Do() { m_howToDo(); }
	inline void Undo() { m_howToUndo(); }

private:
	std::function<void(void)> m_howToDo;
	std::function<void(void)> m_howToUndo;
};

namespace Cmp {
struct History {

public:

	friend class HistoryManager;
	History::History()
		: m_index(-1), m_indexOfCumulfNbOfActions(-1)
	{}

	void beginUndoGroup() {
		m_tmpActionBuffer.resize(0);
	}

	void endUndoGroup() {
		if (m_tmpActionBuffer.size() > 0) {
			//
			m_cumulNbOfActionsToGetToThisUndoGroup.resize(m_indexOfCumulfNbOfActions + 1);
			m_cumulNbOfActionsToGetToThisUndoGroup.push_back((int)m_tmpActionBuffer.size() + (m_indexOfCumulfNbOfActions > -1 ? m_cumulNbOfActionsToGetToThisUndoGroup[m_indexOfCumulfNbOfActions] : 0));
			m_indexOfCumulfNbOfActions++;
			//
			m_actions.resize(m_indexOfCumulfNbOfActions > 0 ? m_cumulNbOfActionsToGetToThisUndoGroup[m_indexOfCumulfNbOfActions - 1] : 0);
			for (size_t i = 0; i < m_tmpActionBuffer.size(); ++i) {
				m_actions.push_back(m_tmpActionBuffer[i]);
				m_index++;
			}
		}
#ifndef NDEBUG
		else {
			spdlog::warn("Empty undo group");
		}
#endif
	}
	void addAction(Action action) {
		m_tmpActionBuffer.push_back(action);
	}

	void moveBackward() {
		if (m_indexOfCumulfNbOfActions > -1) {
			//spdlog::info("moving backward");
			for (size_t i = 0; i < nbOfActionsBetweenThisAndPreviousUndoGroup(m_indexOfCumulfNbOfActions); ++i) {
				m_actions[m_index].Undo();
				m_index--;
			}
			m_indexOfCumulfNbOfActions--;
		}
	}

	void moveForward() {
		if (m_indexOfCumulfNbOfActions < (int)(m_cumulNbOfActionsToGetToThisUndoGroup.size() - 1)) { // cast to an int because size_t is an unsigned type and it causes a bug when m_indexOfCumulfNbOfActions == -1
			//spdlog::info("moving forward");
			for (size_t i = 0; i < nbOfActionsBetweenThisAndPreviousUndoGroup(m_indexOfCumulfNbOfActions + 1); ++i) {
				m_index++;
				m_actions[m_index].Do();
			}
			m_indexOfCumulfNbOfActions++;
		}
	}

	void clear() {
		m_actions.clear();
		m_cumulNbOfActionsToGetToThisUndoGroup.clear();
		m_tmpActionBuffer.clear();
		m_index = -1;
		m_indexOfCumulfNbOfActions = -1;
	}

// Internal

	unsigned int nbOfActionsBetweenThisAndPreviousUndoGroup(int index) {
		return m_cumulNbOfActionsToGetToThisUndoGroup[index] - (index > 0 ? m_cumulNbOfActionsToGetToThisUndoGroup[index - 1] : 0);
	}

	std::vector<Action> m_actions;
	std::vector<int> m_cumulNbOfActionsToGetToThisUndoGroup;
	std::vector<Action> m_tmpActionBuffer;
	int m_index;
	int m_indexOfCumulfNbOfActions;


//Serialization
	friend class cereal::access;
	template<class Archive>
	void save(Archive& archive) const
	{
	}

	template<class Archive>
	void load(Archive& archive)
	{
		m_index = -1;
		m_indexOfCumulfNbOfActions = -1;
	}
};
}